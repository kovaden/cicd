# bayan "баян" - duplicate files finder

This tool recursively scans directories and finds files with the same content.

Usage:
```
bayan [options]
```
 
Options:
* **-h** [ --help ]            This text
* **-i** [ --include ] arg     Scan directory (may appear several times)
* **-e** [ --exclude ] arg     Skip (exclude) directory (may appear several times)
* **--depth arg** (=-1)        scanning depth, default is unlimited
* **--min_size** arg (=1)      Minimum file size for scanning. Default is 1
* **--filemask arg**           File mask
* **--block_size arg** (=4096) Block size to read files
* **--hash_alg arg** (=CRC32)  Hash algorithm used to compare files. Supported
algorihms: CRC32 MD5
