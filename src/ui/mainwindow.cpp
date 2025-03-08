#include "mainwindow.h"

#include <QAction>
#include <QCoreApplication>
#include <QIcon>
#include <QLocale>
#include <QMenu>
#include <QPoint>
#include <QRect>
#include <QScreen>
#include <Qt>
#include <QtGlobal>

#include "../core/indexer.h"
#include "../core/settings.h"
#include "./ui_mainwindow.h"
#include "searchresultlist.h"
#include "settingswindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{parent}, ui_{std::make_unique<Ui::MainWindow>()} {
  ui_->setupUi(this);
  setAttribute(Qt::WA_TranslucentBackground);

  // Prevents child widgets from changing the width of the window.
  setMaximumWidth(width());

  // Everything related to the Theme instance needs to be set here in order for
  // the SearchBox to have the visually correct height. I'm not entirely sure
  // why, however.
  auto& theme = Theme::GetInstance();
  connect(&theme, &Theme::Changed, this, &MainWindow::ApplyTheme);
  connect(&theme, &Theme::Changed, ui_->search_box, &SearchBox::ApplyTheme);
  connect(&theme, &Theme::Changed, ui_->search_list,
          &SearchResultList::ApplyTheme);

  auto& settings = Settings::GetInstance();
  theme.LoadFile(settings.GetThemeFilename());

  // Prevents the window height from strangely expanding when input is
  // cleared.
  // Must be called at the end because the SearchBox component needs to be
  // stylized first.
  setMinimumHeight(ui_->search_box->Height());

  UpdateTranslators();
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

void MainWindow::Show() {
  // Settings should have already been initialized by this point.
  auto& settings = Settings::GetInstance();

  connect(this, &MainWindow::Deactivated, ui_->search_box, &SearchBox::Clear);
  connect(ui_->search_list, &SearchResultList::ItemsChanged, this,
          &MainWindow::SetHeight);
  connect(ui_->search_box, &SearchBox::TextChanged, ui_->search_list,
          &SearchResultList::ProcessText);
  connect(ui_->search_box, &SearchBox::KeyPressed, this,
          &MainWindow::ProcessKeyPress);
  connect(ui_->search_box, &SearchBox::KeyPressed, ui_->search_list,
          &SearchResultList::ProcessKeyPress);
  connect(ui_->search_box, &SearchBox::KeyReleased, ui_->search_list,
          &SearchResultList::ProcessKeyRelease);
  connect(&settings, &Settings::LocaleChanged, this,
          &MainWindow::UpdateTranslators);

  connect(ui_->search_list, &SearchResultList::NewSearchBoxTextRequested,
          ui_->search_box, &SearchBox::SetText);
  connect(ui_->search_list, &SearchResultList::HideRequested, this,
          &MainWindow::Hide);

  // Displays the actual window. This was previously invoked in the main.cpp
  // file.
  show();
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

void MainWindow::ApplyTheme(Theme* theme) {
  // Because the icons are hard-coded whenever the indexer is initialized, if
  // the user changes the theme from a dark theme to a light theme, then the
  // indexer must be reinitialized in order for the appropriate icons to be
  // applied.
  auto& indexer = Indexer::GetInstance();
  indexer.Clear();
  indexer.Initialize();
}

void MainWindow::Hide() {
  hide();
  emit Deactivated();
  setWindowOpacity(1);
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

void MainWindow::ProcessKeyPress(const QKeyCombination& combination) {
  auto key = combination.key();
  switch (key) {
    case Qt::Key_Escape:
      Hide();
      break;
    case Qt::Key_T:
      if (combination.keyboardModifiers() & Qt::ControlModifier) {
        setWindowOpacity(windowOpacity() == 1 ? 0.4 : 1);
      }

      break;
    default:
      break;
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
  auto territory = settings.GetTerritory();
  auto partial_filename =
    QString{":/translations/%1_%2.qm"}.arg(a->applicationName().toLower());

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
