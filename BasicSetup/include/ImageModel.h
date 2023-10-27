#pragma once

#include "DataManager.h"

#include "ImageProvider.h"

#include <QAbstractListModel>
#include <QObject>
#include <QUrl>

class ImageModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString active_image_id READ getActiveImageId NOTIFY activeImageChanged)
public:
    enum Roles
    {
        IMAGE_ID = Qt::UserRole + 1,
        IMAGE_PATH,
        HAS_ANNOTATIONS,
        IS_IMAGE_SELECTED
    };

    ImageModel(DataManagerImpl& dataManager);
    ~ImageModel() override = default;

    int rowCount(const QModelIndex &parent = QModelIndex{}) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void loadFromFolder(const QString& path);

    Q_INVOKABLE void loadDraggedDroppedImages(const QList<QUrl>& paths);

    ImageProvider* getImageProvider() const;

signals:
    void activeImageChanged();

private:
    void loadModel();
    void handleActiveImageChanged(std::shared_ptr<Image> image);
    void handleActiveImageAboutToChange(std::shared_ptr<Image> image);
    void handleImageAdded(std::shared_ptr<Image> image);
    void handleAnnotationAdded(std::shared_ptr<Annotation> annotation);
    void handleAnnotationRemoved(std::shared_ptr<Annotation> annotation);

    void removeImages();
    QModelIndex getIndex(std::shared_ptr<Image> img);

    QString getActiveImageId() const;

    std::vector<std::shared_ptr<Image>> m_images;
    DataManagerImpl& m_data_manager;
    void setupConnection();
    //This must be a raw pointer as it's deleted by Qt itself later
    ImageProvider* m_image_provider;
};
