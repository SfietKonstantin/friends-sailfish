clientId.files = $$PWD/libharbour-friends-clientidplugin.so
clientId.path = $$DEPLOYMENT_PATH/lib
INSTALLS += clientId

data.files = $$PWD/changelog.json \
    $$PWD/sfiet_konstantin.jpg \
    $$PWD/morpog.jpeg \
    $$PWD/logo.png
data.path = $$DEPLOYMENT_PATH/data
INSTALLS += data
