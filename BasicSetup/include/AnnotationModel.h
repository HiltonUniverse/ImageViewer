#pragma once

#include "Annotation.h"
#include "DataManager.h"

#include <QAbstractListModel>
#include <QObject>

class AnnotationModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles
    {
        ANNOTATION_ID = Qt::UserRole + 1,
        ANNOTATION_NOTIFICATION,
    };

    AnnotationModel(DataManagerImpl& dataManager);
    ~AnnotationModel() override = default;

    int rowCount(const QModelIndex & parent = {}) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

private:
    void setupConnection();

    std::vector<std::shared_ptr<Annotation>> m_annotations;
    DataManagerImpl& m_data_manager;
};
