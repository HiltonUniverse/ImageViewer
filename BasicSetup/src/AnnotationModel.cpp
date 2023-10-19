#include "AnnotationModel.h"
#include "Image.h"

//-----------------------------------
AnnotationModel::AnnotationModel(DataManagerImpl& dataManager)
    : m_data_manager(dataManager)

{
    setupConnection();
}

//-----------------------------------
void AnnotationModel::setupConnection()
{
    connect(&m_data_manager, &DataManagerImpl::activeImageChanged, this, &AnnotationModel::handleActiveImageChanged);
    connect(&m_data_manager, &DataManagerImpl::annotationAdded, this, &AnnotationModel::addAnnotationAndAttachObserver);
    connect(&m_data_manager, &DataManagerImpl::annotationRemoved, this, &::AnnotationModel::removeAnnotationAndDetachObserver);
}

//-----------------------------------
int AnnotationModel::rowCount([[maybe_unused]]const QModelIndex & parent) const
{
    return static_cast<int>(m_annotations.size());
}

//-----------------------------------
QVariant AnnotationModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return {};
    }

    const int got_role = static_cast<Roles>(role);
    const auto annotation = m_annotations.at(index.row());
    switch(got_role)
    {
        case Roles::ANNOTATION_ID:
            return annotation->getId();
        case Roles::ANNOTATION_NOTIFICATION:
            return annotation->getNotification();
        case Roles::ANNOTATION_SELECTED:
            return annotation->isSelected();
        case Roles::ANNOTATION_HOVERED:
            return annotation->isHovered();
    }

    Q_UNREACHABLE();

    return {};
}

//-----------------------------------
bool AnnotationModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
    {
        return {};
    }

    const int got_role = static_cast<Roles>(role);
    const auto annotation = m_annotations.at(index.row());
    switch(got_role)
    {
        case Roles::ANNOTATION_ID:
        case Roles::ANNOTATION_NOTIFICATION:
            return false;
        case Roles::ANNOTATION_SELECTED:
        {
            if(m_currently_selected)
            {
                m_currently_selected->setSelected(false);
            }

            annotation->setSelected(value.toBool());
            m_currently_selected = annotation.get();

            emit dataChanged(index, index, {Roles::ANNOTATION_SELECTED});
            return true;
        }
        case Roles::ANNOTATION_HOVERED:
        {
            annotation->setHovered(value.toBool());
            return true;
        }
    }

    Q_UNREACHABLE();

    return {};
}

//-----------------------------------
QHash<int, QByteArray> AnnotationModel::roleNames() const
{
    return
    {
        {Roles::ANNOTATION_ID, {"annotation_id"}},
        {Roles::ANNOTATION_NOTIFICATION, {"annotation_notification"}},
        {Roles::ANNOTATION_SELECTED, {"annotation_selected"}},
        {Roles::ANNOTATION_HOVERED, {"annotation_hovered"}}
    };
}

//-----------------------------------
void AnnotationModel::changed(Annotation* type, const AnnotationEvent::EventType& event)
{
    switch(event)
    {
        case AnnotationEvent::EventType::ANNOTATION_SELECTED:
        {
            if(type->isSelected())
            {
                m_currently_selected = type;
            }

            int index = getIndex(type);
            auto model_index = createIndex(index, index);
            emit dataChanged(model_index, model_index, {Roles::ANNOTATION_SELECTED});
            return;
        }
        case AnnotationEvent::EventType::ANNOTATION_HOVERED:
        {
            int index = getIndex(type);
            auto model_index = createIndex(index, index);
            emit dataChanged(model_index, model_index, {Roles::ANNOTATION_HOVERED});
            return;
        }
    }
    Q_UNREACHABLE();
}

//-----------------------------------
int AnnotationModel::getIndex(Annotation *annotation) const
{
    auto itr = std::find_if(m_annotations.begin(), m_annotations.end(), [annotation](const auto& localAnnotation)
    {
        return (annotation->getId() == localAnnotation->getId());
    });

    if(itr == m_annotations.end())
    {
        return -1;
    }

    return std::distance(m_annotations.begin(), itr);
}

//-----------------------------------
void AnnotationModel::addAnnotationAndAttachObserver(std::shared_ptr<Annotation> annotation)
{
    beginInsertRows(QModelIndex(), this->rowCount(), this->rowCount());

    annotation->attach(this);
    m_annotations.push_back(annotation);

    endInsertRows();
}

//-----------------------------------
void AnnotationModel::removeAnnotationAndDetachObserver(std::shared_ptr<Annotation> annotation)
{
    int index = getIndex(annotation.get());
    beginRemoveRows(QModelIndex(), index, index);

    annotation->detach(this);
    m_annotations.erase(std::remove(m_annotations.begin(), m_annotations.end(), annotation), m_annotations.end());

    endRemoveRows();
}

//-----------------------------------
void AnnotationModel::handleActiveImageChanged(std::shared_ptr<Image> activeImage)
{
    beginResetModel();

    for(auto& annotation : m_annotations)
    {
        annotation->detach(this);
    }

    m_annotations.clear();

    for(auto& annotation : activeImage->getAnnotations())
    {
        addAnnotationAndAttachObserver(annotation);
    }

    endResetModel();
}
