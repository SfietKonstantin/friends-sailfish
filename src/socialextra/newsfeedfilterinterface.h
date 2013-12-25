#ifndef NEWSFEEDFILTERINTERFACE_H
#define NEWSFEEDFILTERINTERFACE_H

#include "filterinterface.h"

class NewsFeedFilterInterfacePrivate;
class NewsFeedFilterInterface : public FilterInterface
{
    Q_OBJECT
    Q_PROPERTY(FeedType type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString identifier READ identifier WRITE setIdentifier NOTIFY identifierChanged)
    Q_ENUMS(FeedType)
public:
    enum FeedType {
        Home,
        Feed
    };
    explicit NewsFeedFilterInterface(QObject *parent = 0);
    FeedType type() const;
    void setType(FeedType type);
    QString identifier() const;
    void setIdentifier(const QString &identifier);

    bool isAcceptable(QObject *item, SocialNetworkInterface *socialNetwork) const;
signals:
    void typeChanged();
    void identifierChanged();
protected:
    bool performLoadRequestImpl(QObject *item, SocialNetworkInterface *socialNetwork,
                                LoadType loadType);
    bool performSetModelDataImpl(SocialNetworkModelInterface *model,
                                 SocialNetworkInterface *socialNetwork, const QByteArray &data,
                                 LoadType loadType, const QVariantMap &properties);

private:
    Q_DECLARE_PRIVATE(NewsFeedFilterInterface)
};

#endif // NEWSFEEDFILTERINTERFACE_H
