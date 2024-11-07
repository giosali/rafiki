#include "application.h"

#include <QDesktopServices>
#include <QIcon>
#include <QUrl>
#include <cstdlib>
#include <thread>

#include "../core/crypto.h"
#include "../core/utils.h"

Application::Application(const std::filesystem::path& desktop_entry_path,
                         QSettings& desktop_entry_file) {
  desktop_entry_file.beginGroup("Desktop Entry");
  exec_ =
    RemoveFieldCodes(desktop_entry_file.value("Exec").toString().toStdString());
  auto id = Crypto::Djb2(desktop_entry_path);
  auto name = desktop_entry_file.value("Name").toString();
  auto icon = desktop_entry_file.value("Icon").toString();
  auto description = QString::fromStdString(desktop_entry_path);

  SetId(id);
  SetPixmap(QIcon::fromTheme(icon));
  SetTitle(name);
  SetCommand(name);
  SetDescription(description);

  desktop_entry_file.endGroup();
}

std::unordered_set<std::string> Application::Tokenize() const {
  auto tokens = Result::Tokenize();

  // Keeps track of uppercase letters to convert, for example, ProtonVPN to
  // pvpn.
  auto acronym = QString{};
  auto capitalized_strings = std::vector<QString>{};
  auto capitalized_string = QString{};
  for (auto ch : GetCommand()) {
    if (ch.isUpper()) {
      // Handles acronymizing the command.
      acronym += ch;

      // Handles detecting capitalized words within the command.
      if (!capitalized_string.isEmpty()) {
        capitalized_strings.push_back(capitalized_string);
        capitalized_string.clear();
      }

      capitalized_string += ch;
      continue;
    } else if (ch.isSpace() && !capitalized_string.isEmpty()) {
      // Handles detecting capitalized words within the command.
      capitalized_strings.push_back(capitalized_string);
      capitalized_string.clear();
      continue;
    }

    if (!capitalized_string.isEmpty()) {
      capitalized_string += ch;
    }
  }

  if (!capitalized_string.isEmpty()) {
    capitalized_strings.push_back(capitalized_string);
  }

  // Takes pvpn, for example, and breaks it down to:
  // p, v, p, n
  // pv, vp, pn
  // pvp, vpn
  // pvpn
  for (size_t i = 1, l = acronym.length(); i <= l; ++i) {
    // j += i
    for (size_t j = 0; j + i <= l; ++j) {
      auto token = acronym.sliced(j, i).toLower().toStdString();
      tokens.insert(token);
    }
  }

  for (const auto& string : capitalized_strings) {
    tokens.insert(string.toLower().toStdString());
  }

  return tokens;
}

void Application::Drag() {};

void Application::ProcessKeyPress(const QKeyCombination& combination,
                                  const Input& input) {
  switch (combination.key()) {
    case Qt::Key_Return: {
      if (combination.keyboardModifiers() & Qt::AltModifier) {
        // TODO: inform user that the action did not work.
        auto description = GetDescription();
        if (description.isEmpty()) {
          break;
        }

        emit Hidden();
        auto path = std::filesystem::path{description.toStdString()};
        auto parent = path.parent_path();
        auto dir = QUrl::fromLocalFile(QString::fromStdString(parent));
        QDesktopServices::openUrl(dir);
        break;
      }

      emit Hidden();

      // This does not execute when running through VSCode.
      auto t =
        std::thread{[this]() { std::system(exec_.toStdString().c_str()); }};
      t.detach();
      break;
    }
    case Qt::Key_Alt:
      emit NewDescriptionRequested("Reveal in folder");
      break;
    default:
      break;
  }
}

void Application::ProcessKeyRelease(const QKeyCombination& combination,
                                    const Input& input) {
  switch (combination.key()) {
    case Qt::Key_Alt:
      emit NewDescriptionRequested(GetDescription());
      break;
    default:
      break;
  }
}

QString Application::RemoveFieldCodes(const std::string& exec) const {
  auto parts = utils::Split(exec);
  auto filtered_parts = std::vector<std::string>{};
  for (const auto& part : parts) {
    // https://specifications.freedesktop.org/desktop-entry-spec/desktop-entry-spec-latest.html#exec-variables
    if (part.length() == 2 && part[0] == '%') {
      switch (part[1]) {
        case 'f':
        case 'F':
        case 'u':
        case 'U':
        case 'd':
        case 'D':
        case 'n':
        case 'N':
        case 'i':
        case 'c':
        case 'k':
        case 'v':
        case 'm':
          continue;
      }
    }

    filtered_parts.push_back(part);
  }

  switch (exec[0]) {
    case 'a':
      break;
  }

  return QString::fromStdString(utils::Join(filtered_parts, " "));
}
