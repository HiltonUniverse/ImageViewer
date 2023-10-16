#include "ImageModel.h"
#include "SelectUndoRedo.h"
#include "Image.h"

#include <QDir>


//-----------------------------------
ImageModel::ImageModel(DataManagerImpl& dataManager)
    : m_data_manager(dataManager)
{
    loadModel();
    setupConnection();
}

//-----------------------------------
void ImageModel::loadModel()
{
    auto images = m_data_manager.getImages();

    for(auto& image : images)
    {
        m_images.push_back(image);
    }
}

//-----------------------------------
void ImageModel::setupConnection()
{
    connect(&m_data_manager, &DataManagerImpl::activeImageAboutToChanged, [this](auto image)
    {
        if(!image)
        {
            return;
        }

        auto model_index = getIndex(image);
        image->setSelected(false);
        emit dataChanged(model_index, model_index, {ImageModel::Roles::IS_IMAGE_SELECTED});
    });

    connect(&m_data_manager, &DataManagerImpl::activeImageChanged, [this](auto image)
    {
        if(!image)
        {
            return;
        }

        auto model_index = getIndex(image);
        emit dataChanged(model_index, model_index, {ImageModel::Roles::IS_IMAGE_SELECTED});
    });

    connect(&m_data_manager, &DataManagerImpl::annotationAdded, [this]([[maybe_unused]]auto annotation)
    {
        auto model_index = getIndex(m_data_manager.getActiveImage());
        emit dataChanged(model_index, model_index, {ImageModel::Roles::HAS_ANNOTATIONS});
    });

    connect(&m_data_manager, &DataManagerImpl::annotationRemoved, [this]([[maybe_unused]]auto annotation)
    {
        auto model_index = getIndex(m_data_manager.getActiveImage());
        emit dataChanged(model_index, model_index, {ImageModel::Roles::HAS_ANNOTATIONS});
    });
}

//-----------------------------------
QModelIndex ImageModel::getIndex(std::shared_ptr<Image> img)
{
    if(!img)
    {
        return {};
    }

    auto itr = std::find(m_images.begin(), m_images.end(), img);
    if(itr == m_images.end())
    {
        return {};
    }

    int index = std::distance(m_images.begin(), itr);
    return createIndex(index, index);
}

//-----------------------------------
int ImageModel::rowCount([[maybe_unused]]const QModelIndex &parent) const
{
    return static_cast<int>(m_images.size());
}

//-----------------------------------
QVariant ImageModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return {};
    }

    const int got_role = static_cast<Roles>(role);
    const auto image = m_images.at(index.row());

    switch(got_role)
    {
        case Roles::IMAGE_ID:
            return image->getId();
        case Roles::IMAGE_PATH:
            return image->getPath();
        case Roles::HAS_ANNOTATIONS:
            return image->getAnnotations().size() > 0;
        case Roles::IS_IMAGE_SELECTED:
            return image->isSelected();
    }

    Q_UNREACHABLE();

    return {};
}

//-----------------------------------
bool ImageModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
    {
        return {};
    }

    const int got_role = static_cast<Roles>(role);
    const auto image = m_images.at(index.row());
    switch(got_role)
    {
        case Roles::IMAGE_ID:
        case Roles::HAS_ANNOTATIONS:
            return false;
        case Roles::IMAGE_PATH:
        {
            auto select_undo_redo = std::make_unique<SelectUndoRedo>( image,
                                                                 m_data_manager);

            m_data_manager.m_undo_stack.push(select_undo_redo.release());
            return true;
        }
        case Roles::IS_IMAGE_SELECTED:
        {
            image->setSelected(value.toBool());
            emit dataChanged(index, index, {Roles::IS_IMAGE_SELECTED});
            return true;
        }
    }

    Q_UNREACHABLE();

    return {};
}

//-----------------------------------
QHash<int, QByteArray> ImageModel::roleNames() const
{
    return
    {
        {Roles::IMAGE_ID, {"image_id"}},
        {Roles::IMAGE_PATH, {"image_path"}},
        {Roles::HAS_ANNOTATIONS, {"has_annotations"}},
        {Roles::IS_IMAGE_SELECTED, {"is_image_selected"}},
    };
}

//-----------------------------------
void ImageModel::loadFromFolder(const QString &path)
{
    QString modified_path = path;
    modified_path.remove("file://");

    //browse files inside the path
    QDir directory(modified_path);
    QStringList name_list = directory.entryList(QDir::Files);

    beginResetModel();
    for(auto &name : name_list)
    {
        const QString path = directory.absoluteFilePath(name).prepend("file://");
        auto image = std::make_shared<Image>(name, path);
        m_data_manager.addImage(image);
    }
    endResetModel();
}

//-----------------------------------
void ImageModel::loadDraggedDroppedImages(const QList<QUrl> &paths)
{
    for(const QUrl& path : paths)
    {
        std::make_shared<Image>(path.fileName(), path.toDisplayString());
    }
}
