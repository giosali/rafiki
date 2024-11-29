#include "applicationmodel.h"

#include <QRegularExpression>
#include <memory>

#include "../bridges/applicationbridge.h"
#include "../crypto.h"
#include "../utilities.h"
#include "../visitors/featurevisitor.h"

ApplicationModel::ApplicationModel(
  const std::filesystem::path& desktop_entry_path, const INIReader& reader)
    : FeatureModel{std::make_unique<ApplicationBridge>()} {
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
  auto keywords = Utilities::Split(QString::fromStdString(keywords_val), ';');

  SetAltDescription("Open containing folder");
  SetCommand(name);
  SetDescription(QString::fromStdString(desktop_entry_path));
  SetIcon(QIcon::fromTheme(icon));
  SetId(Crypto::Djb2(desktop_entry_path));
  SetTitle(name);

  exec_ = exec;
  keywords_ = keywords;
}

void ApplicationModel::Accept(FeatureVisitor& v) { v.Visit(this); }

std::unordered_set<std::string> ApplicationModel::Tokenize() const {
  auto tokens = FeatureModel::Tokenize();

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

QString ApplicationModel::GetExec() const { return exec_; }
