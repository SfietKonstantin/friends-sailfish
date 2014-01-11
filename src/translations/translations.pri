# Now managed by the release script

engineering_english_install.path = $$DEPLOYMENT_PATH/translations
engineering_english_install.files = $$PWD/friends-engineering-english.qm

translations_install.path = $$DEPLOYMENT_PATH/translations
translations_install.files = $$PWD/friends_*.qm

INSTALLS += engineering_english_install
INSTALLS += translations_install
