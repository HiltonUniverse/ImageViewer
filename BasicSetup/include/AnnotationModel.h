#pragma once

#include "Annotation.h"
#include "DataManager.h"
#include "AnnotationEvent.h"
#include "ObserverPattern.h"

#include <QAbstractListModel>
#include <QObject>

class AnnotationModel : public QAbstractListModel
                      , public pattern::Observer<Annotation, AnnotationEvent::EventType>
{
    Q_OBJECT
public:
    enum Roles
    {
        ANNOTATION_ID = Qt::UserRole + 1,
        ANNOTATION_NOTIFICATION,
        ANNOTATION_SELECTED,
        ANNOTATION_HOVERED
    };

    AnnotationModel(DataManagerImpl& dataManager);
    ~AnnotationModel() override = default;

    int rowCount(const QModelIndex & parent = {}) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    QHash<int, QByteArray> roleNames() const override;

    void changed(Annotation* type, const AnnotationEvent::EventType& event) override;

private:

    int getIndex(Annotation* annotation) const;
    void addAnnotationAndAttachObserver(std::shared_ptr<Annotation> annotation);
    void removeAnnotationAndDetachObserver(std::shared_ptr<Annotation> annotation);
    void handleActiveImageChanged(std::shared_ptr<Image> activeImage);
    void setupConnection();

    std::vector<std::shared_ptr<Annotation>> m_annotations;
    Annotation* m_currently_selected;
    DataManagerImpl& m_data_manager;
};
