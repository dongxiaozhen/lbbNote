sstable
---------------
|data block 1  |
|--------------|
|data block 2  |
|--------------|
|...           |
|--------------|
|filter block  |
|--------------|
|metaindex blk |
|--------------|
| index block  |
|--------------|
|footer        |
----------------


data block
--------------
|record  1|
|---------|
|...      | group 1
|recodr 16|
|---------|--
|record 17|
|---------|
|...      | group 2
|recodr 32|
|---------|--
|...      |
|---------|
|restart 0|
|---------|
|restart 1|
|---------|
|restart 2|
|---------|
|num restart|
|---------|
|type     |
|---------|
|crc32    |
|---------|


footer
|--------------|
|metaindex_block_handle|
|--------------|
|index_block_handle|
|--------------|
|padding       |
|--------------|
|magic number  |
|--------------|

