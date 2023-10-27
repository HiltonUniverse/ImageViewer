#pragma once

#include <QQuickImageProvider>

#include <unordered_map>

class ImageProvider : public QQuickImageProvider
{
public:
    //-----------------------------------
    ImageProvider()
        : QQuickImageProvider(QQmlImageProviderBase::Image)
    {
    }

    //-----------------------------------
    ~ImageProvider() override = default;

    //-----------------------------------
    void addImage(const QString& id, const QImage& image)
    {
        if(m_images.count(id))
        {
            return;
        }

        m_images[id] = image;
    }

    //-----------------------------------
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override
    {
        return m_images[id];
    }

private:
    std::unordered_map<QString, QImage> m_images;
};
