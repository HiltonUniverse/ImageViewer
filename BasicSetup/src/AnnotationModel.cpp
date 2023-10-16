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
    connect(&m_data_manager, &DataManagerImpl::activeImageChanged, [this](auto image)
    {
        beginResetModel();

        m_annotations.clear();

        for(auto& annotation : image->getAnnotations())
        {
            m_annotations.push_back(annotation);
        }

        endResetModel();
    });

    connect(&m_data_manager, &DataManagerImpl::annotationAdded, [this](auto annotation)
    {
        beginInsertRows(QModelIndex(), this->rowCount(), this->rowCount());
        m_annotations.push_back(annotation);
        endInsertRows();
    });

    connect(&m_data_manager, &DataManagerImpl::annotationRemoved, [this](auto annotation)
    {
        auto itr = std::find(m_annotations.begin(), m_annotations.end(), annotation);
        if(itr == m_annotations.end())
        {
            return;
        }
        int index = std::distance(m_annotations.begin(), itr);

        beginRemoveRows(QModelIndex(), index, index);
        m_annotations.erase(std::remove(m_annotations.begin(), m_annotations.end(), annotation), m_annotations.end());
        endRemoveRows();
    });
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
        {Roles::ANNOTATION_NOTIFICATION, {"annotation_notification"}}
    };
}
