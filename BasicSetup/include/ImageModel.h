#pragma once

#include "Annotation.h"
#include "DataManager.h"

#include <QAbstractListModel>
#include <QObject>

class ImageModel : public QAbstractListModel
{
    Q_OBJECT
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

    int rowCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    QHash<int, QByteArray> roleNames() const override;

private:
    void loadModel();
    QModelIndex getIndex(std::shared_ptr<Image> img);

    std::vector<std::shared_ptr<Image>> m_images;
    DataManagerImpl& m_data_manager;
    void setupConnection();
};
