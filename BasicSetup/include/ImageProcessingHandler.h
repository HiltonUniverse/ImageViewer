#pragma once
#include "DataManager.h"

class ImageProcessingHandler : public QObject
{
    Q_OBJECT
public:
    explicit ImageProcessingHandler(DataManagerImpl& dataManager);
    ~ImageProcessingHandler() override = default;

    Q_INVOKABLE void convertToGrayscale();
    Q_INVOKABLE void convertToBinary();
    Q_INVOKABLE void convertToInverseBinary();
    Q_INVOKABLE void wrapActiveImage();

private:
    DataManagerImpl& m_datamanager;
};
