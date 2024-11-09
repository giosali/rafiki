#include "application.h"

#include <QDesktopServices>
#include <QIcon>
#include <QRegularExpression>
#include <QUrl>
#include <cstdlib>
#include <string>
#include <thread>

#include "../core/crypto.h"
#include "../core/utilities.h"

Application::Application(const std::filesystem::path& desktop_entry_path,
                         const INIReader& reader) {
  // https://specifications.freedesktop.org/desktop-entry-spec/latest/exec-variables.html
  auto section = "Desktop Entry";
  auto name_val = reader.Get(section, "Name", {});
  auto icon_val = reader.Get(section, "Icon", {});
  auto exec_val = reader.Get(section, "Exec", {});
  auto keywords_val = reader.Get(section, "Keywords", {});

  auto name = QString::fromStdString(name_val);
  auto icon = QString::fromStdString(icon_val);
  auto exec = QString::fromStdString(exec_val).remove(
    QRegularExpression{"%[fFuUdDnNickvm]"});
  keywords_ = Utilities::Split(QString::fromStdString(keywords_val), ';');

  SetId(Crypto::Djb2(desktop_entry_path));
  SetPixmap(QIcon::fromTheme(icon));
  SetTitle(name);
  SetCommand(name);
  SetDescription(QString::fromStdString(desktop_entry_path));
}

std::unordered_set<std::string> Application::Tokenize() const {
  auto tokens = Result::Tokenize();

  for (const auto& keyword : keywords_) {
    tokens.insert(keyword.toLower().toStdString());
  }

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
