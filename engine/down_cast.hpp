#pragma once

template <typename DestType, typename SourceType>
inline DestType down_cast(SourceType pointer)
{
#if 0
    return dynamic_cast<DestType>(pointer);
#else
    return static_cast<DestType>(pointer);
#endif
}
