blockSize  = 32 * 1024

每个block 开头7个字节是 checksum，length，chunkType,

chunType 类型
    fullChunkType   = 1, 说明一个block包含所有信息
    firstChunkType  = 2, 说明这是第一个block，至少还有一个
    middleChunkType = 3
    lastChunkType   = 4
