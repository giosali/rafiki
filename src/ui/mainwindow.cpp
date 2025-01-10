#include "mainwindow.h"

#include <qcoreapplication.h>

#include <QAction>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QIcon>
#include <QLocale>
#include <QMenu>
#include <QPoint>
#include <QRect>
#include <QScreen>
#include <Qt>
#include <QtGlobal>

#include "../core/settings.h"
#include "./ui_mainwindow.h"
#include "searchbox.h"
#include "searchresultlist.h"
#include "settingswindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{parent}, ui_{std::make_unique<Ui::MainWindow>()} {
  ui_->setupUi(this);

  setAttribute(Qt::WA_TranslucentBackground);
  auto& theme = Theme::GetInstance();
  auto& settings = Settings::GetInstance();
  theme.LoadFile(settings.GetThemeFilename());
  ApplyTheme(&theme);

  // Prevents child widgets from changing the width of the window.
  setMaximumWidth(width());

  auto box = new SearchBox(this);
  auto list = new SearchResultList(box, this);
  list->ApplyTheme(&theme);

  // Prevents the window height from strangely expanding when input is cleared.
  setMinimumHeight(box->Height());

  connect(this, &MainWindow::Deactivated, box, &SearchBox::Clear);
  connect(list, &SearchResultList::ItemsChanged, this, &MainWindow::SetHeight);
  connect(box, &SearchBox::TextChanged, list, &SearchResultList::ProcessText);
  connect(box, &SearchBox::KeyPressed, list,
          &SearchResultList::ProcessKeyPress);
  connect(box, &SearchBox::KeyReleased, list,
          &SearchResultList::ProcessKeyRelease);
  connect(&theme, &Theme::Changed, this, &MainWindow::ApplyTheme);
  connect(&theme, &Theme::Changed, list, &SearchResultList::ApplyTheme);
  connect(&settings, &Settings::LocaleChanged, this,
          &MainWindow::UpdateTranslators);

  centralWidget()->layout()->addWidget(box);
  centralWidget()->layout()->addWidget(list);
}

MainWindow::~MainWindow() {}

void MainWindow::CreateTrayIcon() {
  if (!QSystemTrayIcon::isSystemTrayAvailable()) {
    return;
  }

  auto tray_menu = new QMenu{};

  // Handles Settings action.
  auto settings_action = new QAction{"Settings", this};
  connect(settings_action, &QAction::triggered, this,
          &MainWindow::OpenSettingsWindow);
  tray_menu->addAction(settings_action);

  // Handles Quit action.
  auto quit_action = new QAction{"Quit", this};
  connect(quit_action, &QAction::triggered, this, &QCoreApplication::quit);
  tray_menu->addAction(quit_action);

  auto tray_icon = new QSystemTrayIcon{this};
  tray_icon->setIcon(QIcon{":/icons/rafiki.png"});
  connect(tray_icon, &QSystemTrayIcon::activated, this,
          &MainWindow::ProcessActivationReason);
  tray_icon->setContextMenu(tray_menu);
  tray_icon->show();
}

void MainWindow::ApplyTheme(Theme* theme) {
  auto stylesheet =
    QString{"QWidget { background-color: %1; border-radius: %2px; }"}
      .arg(theme->GetWindowBackgroundColor().name())
      .arg(theme->GetBorderRadius());
  centralWidget()->setStyleSheet(stylesheet);
}

void MainWindow::Hide() {
  hide();
  emit Deactivated();
}

void MainWindow::OpenSettingsWindow() {
  Hide();
  (new SettingsWindow{this})->show();
}

void MainWindow::ProcessActivationReason(
  QSystemTrayIcon::ActivationReason reason) {
  switch (reason) {
    case QSystemTrayIcon::ActivationReason::MiddleClick:
      ToggleVisibility();
      break;
    default:
      break;
  }
}

void MainWindow::ProcessCommandLineArguments(const QStringList& arguments,
                                             bool from_server) {
  auto parser = QCommandLineParser{};

  // Sets up default options.
  parser.addHelpOption();
  parser.addVersionOption();

  // Sets up custom options.
  auto toggle_option = QCommandLineOption{
    "toggle", "Toggles the visibility of the main input window."};
  parser.addOption(toggle_option);
  auto quit_option = QCommandLineOption{"quit", "Quits the application."};
  parser.addOption(quit_option);

  // It's necessary to parse arguments this way rather than use
  // `parser.process(app)` because the `process` function won't handle
  // arguments passed via the local socket.
  if (!parser.parse(arguments)) {
    return;
  }

  if (parser.isSet("help")) {
    parser.showHelp();
  } else if (parser.isSet("version")) {
    parser.showVersion();
  }

  // The custom options shouldn't be handled if the args aren't from the server.
  if (!from_server) {
    return;
  }

  if (parser.isSet(toggle_option)) {
    ToggleVisibility();
  }

  if (parser.isSet(quit_option)) {
    QCoreApplication::quit();
  }
}

void MainWindow::SetHeight(int height) {
  // The minimum height of the window being already set prevents the window from
  // shrinking down all the way to 0, which is a good thing.
  resize(width(), minimumHeight() + height);
}

void MainWindow::UpdateTranslators() {
  auto a = QCoreApplication::instance();
  a->removeTranslator(&language_translator_);
  a->removeTranslator(&territory_translator_);

  auto& settings = Settings::GetInstance();
  auto partial_filename =
    QString{":/translations/%1_%2.qm"}.arg(a->applicationName().toLower());
  auto territory = settings.GetTerritory();

  if (auto filename =
        partial_filename.arg(QLocale{settings.GetLanguage(), territory}.name());
      language_translator_.load(filename)) {
    a->installTranslator(&language_translator_);
  }

  if (auto filename = partial_filename.arg(QLocale::territoryToCode(territory));
      territory_translator_.load(filename)) {
    a->installTranslator(&territory_translator_);
  }
}

void MainWindow::changeEvent(QEvent* event) {
  switch (event->type()) {
    case QEvent::LanguageChange: {
      // Reloads web search models with tr() function calls to ensure their
      // translations are also updated.
      auto& indexer = Indexer::GetInstance();
      indexer.Clear();
      indexer.Initialize();
      break;
    }
    default:
      break;
  }
}

bool MainWindow::event(QEvent* event) {
  switch (event->type()) {
    case QEvent::WindowActivate: {
      // Repositions the window whenever it becomes activated.
      // I opted for `screen()->size()` rather than `screen()->geometry()`
      // because only the `width` and `height` are relevant. The `x` and `y`
      // properties don't matter and seem to always be `0,0`; I'm not sure if
      // they're always `0,0` though.
      // On Linux, this only works on X11 or XWayland.
      auto screen_size = screen()->size() / 2;  // "Centers" the size.
      auto window_size = frameSize() / 2;       // "Centers" the size.
      auto screen_point = QPoint{screen_size.width(), screen_size.height()};
      auto window_point = QPoint{window_size.width(), window_size.height()};
      auto point = screen_point - window_point;
      point.setY(static_cast<int>(screen_size.height() * 0.35));
      move(point);
      break;
    }
    case QEvent::WindowDeactivate:  // Window lost focus.
      if (!isHidden()) {
        Hide();
      }

      break;
    default:
      break;
  }

  return QMainWindow::event(event);
}

void MainWindow::ToggleVisibility() {
  if (isHidden()) {
    show();
    activateWindow();
    raise();
  } else {
    Hide();
  }
}
