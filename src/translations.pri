# translations
TS_FILE = $$PWD/translations/friends.ts
!CONFIG(desktop):{
EE_QM = $$OUT_PWD/friends-engineering-english.qm
} else {
EE_QM = $$PWD/translations/friends-engineering-english.qm
}

ts.commands += lupdate $$PWD -ts $$TS_FILE
ts.CONFIG += no_check_exist
ts.output = $$TS_FILE
ts.input = .

# We don't install TS file (for now)
#ts_install.files = $$TS_FILE
#ts_install.path = /usr/share/translations/source
#ts_install.CONFIG += no_check_exist

QMAKE_EXTRA_TARGETS += ts
PRE_TARGETDEPS += ts

# Engineering english
engineering_english.commands += lrelease -idbased $$TS_FILE -qm $$EE_QM
engineering_english.CONFIG += no_check_exist
engineering_english.depends = ts

engineering_english_install.path = $$DEPLOYMENT_PATH/translations
engineering_english_install.files = $$EE_QM
engineering_english_install.CONFIG += no_check_exist

QMAKE_EXTRA_TARGETS += engineering_english
PRE_TARGETDEPS += engineering_english

# Other translations
translations.commands += $$PWD/translations/translate.sh $$PWD/translations/ $$OUT_PWD
translations.CONFIG += no_check_exist
translations.depends = engineering_english

translations_install.path = $$DEPLOYMENT_PATH/translations
translations_install.files = $$OUT_PWD/friends_*.qm
translations_install.CONFIG += no_check_exist

QMAKE_EXTRA_TARGETS += translations
PRE_TARGETDEPS += translations

#INSTALLS += ts_install
INSTALLS += engineering_english_install
INSTALLS += translations_install
