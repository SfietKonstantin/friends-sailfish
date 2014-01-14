# Now managed by the release script

engineering_english_install.path = $$DEPLOYMENT_PATH/translations
engineering_english_install.files = $$PWD/friends-engineering-english.qm

translations_install.path = $$DEPLOYMENT_PATH/translations
translations_install.files = $$PWD/friends_*.qm

INSTALLS += engineering_english_install
INSTALLS += translations_install

# Still build TS and QM on Desktop
CONFIG(desktop): {
TS_FILE = $$OUT_PWD/friends.ts
EE_QM = $$PWD/friends-engineering-english.qm

ts.commands += lupdate $$PWD/../ -ts $$TS_FILE
ts.CONFIG += no_check_exist
ts.output = $$TS_FILE
ts.input = .

QMAKE_EXTRA_TARGETS += ts
PRE_TARGETDEPS += ts

# Engineering english
engineering_english.commands += lrelease -idbased $$TS_FILE -qm $$EE_QM
engineering_english.CONFIG += no_check_exist
engineering_english.depends = ts

QMAKE_EXTRA_TARGETS += engineering_english
PRE_TARGETDEPS += engineering_english
}
