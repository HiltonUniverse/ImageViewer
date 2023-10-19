#pragma once

struct AnnotationEvent
{
    enum class EventType
    {
        ANNOTATION_SELECTED,
        ANNOTATION_HOVERED,
        ANNOTATION_PATH_UPDATED
    };
};
