#pragma once

#define LOGGER(__level, __func, __msg) \
    call log(__level, __FILE__, __LINE__, __func, __msg);
