#pragma once

// 文件路径最大长度，部分老旧Windows可能限制在260。此处暂不考虑。
#define MAX_FILE_PATH_LENGTH 1024

// URL最大长度
#define MAX_URL_LENGTH 1024

#define MAX_SYMBOL_FILE_LOCAL_HISTORY_COUNT 10

// 计算数组元素个数
#define ELEMENT_OF(x) (sizeof(x) / sizeof(x[0]))