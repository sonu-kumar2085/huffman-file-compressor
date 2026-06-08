# 🗜️ Huffman File Compressor

A **lossless file compression tool** built in C++ using the **Huffman Encoding algorithm**. It compresses text and image files into a `.huff` format and can reconstruct the original file byte-for-byte.

---

## 📌 Overview

Huffman Coding is a **greedy algorithm** used for lossless data compression. It assigns shorter binary codes to more frequently occurring characters and longer codes to less frequent ones, reducing the overall file size.

This project implements the full pipeline:
1. **Frequency Analysis** → Count how often each byte appears in the file
2. **Huffman Tree Construction** → Build an optimal binary tree using a min-heap (priority queue)
3. **Code Generation** → Traverse the tree to assign unique binary codes to each byte
4. **Encoding** → Compress the file by writing packed bits + a frequency table header
5. **Decoding** → Rebuild the tree from the header and reconstruct the original file

---

## 📁 Project Structure

```
Compressor/
├── main.cpp            # Entry point — detects file type and routes to encode/decode
├── frequency.h         # Header for frequency counter
├── frequency.cpp       # Counts byte frequency from the input file
├── huffman.h           # Header for Huffman Node struct and tree builder
├── huffman.cpp         # Builds the Huffman tree using a min-heap priority queue
├── encoder.h           # Header for code generation and encoder
├── encoder.cpp         # Generates Huffman codes and encodes the file
├── decoder.h           # Header for the decoder
├── decoder.cpp         # Decodes .huff file and reconstructs original text file
├── image.h             # Header for image compress/decompress
├── image.cpp           # Image compression using pixel-level Huffman encoding
├── stb_image.h         # Single-header image loader (JPG, PNG, BMP, etc.)
└── stb_image_write.h   # Single-header image writer (PNG output)
```

---

## ⚙️ How It Works

### Text Compression (Encoding)
1. The input file is read byte-by-byte and a **frequency table** (size 256) is built.
2. A **min-heap** is used to build the Huffman Tree — the two nodes with the lowest frequency are merged repeatedly until a single root remains.
3. The tree is traversed recursively to assign binary codes (`0` for left, `1` for right) to each byte.
4. The output `.huff` file is written with:
   - A **frequency table header** (256 × 4 bytes = 1024 bytes) for reconstruction
   - A **1-byte padding field** to handle non-multiple-of-8 bit lengths
   - **Packed bit data** (the compressed content)

### Text Decompression (Decoding)
1. The frequency table is read from the `.huff` file header.
2. The Huffman Tree is **reconstructed** from that table.
3. The compressed bits are traversed through the tree to recover each original byte.
4. The reconstructed file is saved as `Uncomp_<originalname>` in the same folder.

### Image Compression
1. The image is loaded into **raw pixel bytes** using `stb_image`.
2. A frequency table is built over all pixel values (0–255).
3. A Huffman tree is built and each pixel byte is encoded into packed bits.
4. The `.huff` file stores: `width` + `height` + `channels` + `freq[256]` + `padding` + encoded bits.

### Image Decompression
1. The header (dimensions + frequency table + padding) is read back.
2. The Huffman tree is rebuilt and bits are decoded back into pixel bytes.
3. The output is saved as `Uncomp_<originalname>.png` in the same folder using `stb_image_write`.

---

## ⚠️ Image Compression Limitation

> **JPEG and PNG images do NOT compress well with Huffman encoding.**

Here's why:

- **JPEG** files are already compressed using DCT (Discrete Cosine Transform) + their own entropy encoding. The raw binary of a `.jpg` file has a nearly **uniform byte distribution** — almost every value (0–255) appears with similar frequency.
- **PNG** files use DEFLATE (LZ77 + Huffman) internally — they are already Huffman-compressed.

Because Huffman coding relies on **byte frequency skew** (some bytes appearing much more than others) to assign shorter codes, it gains almost **no advantage** on already-compressed data.

Additionally, the compressor first **decodes the image to raw pixels** (e.g., a 1000×1000 RGB JPEG → 3,000,000 raw bytes), then applies Huffman on those raw pixels. This raw pixel data is much larger than the original JPEG, so:

```
test.jpg         →   3.3 MB    (original JPEG, already compressed)
test.jpg.huff    →  ~63 MB    (raw pixels Huffman encoded — much larger ❌)
```

**Huffman works best on:**
- Plain text files (`.txt`, `.log`, `.csv`) — where some characters repeat far more than others
- BMP / raw image formats — where pixel data is uncompressed and has skewed distributions

---

## 🚀 Getting Started

### Prerequisites
- A C++ compiler (g++ recommended)
- C++17 or later

### Compilation

```bash
g++ -o compressor main.cpp frequency.cpp huffman.cpp encoder.cpp decoder.cpp image.cpp -std=c++17
```

### Usage

```bash
./compressor <filename or full/path/to/file>
```

The tool **auto-detects** the file type by extension:

| Input Extension | Action |
|-----------------|--------|
| `.txt`          | Compress → produces `<file>.txt.huff` |
| `.jpg` / `.png` | Compress image → produces `<file>.jpg.huff` |
| `.huff`         | Decompress → produces `Uncomp_<file>` in same folder |

#### Examples

```bash
# Compress a text file
./compressor Testing/sonu.txt
# → produces Testing/sonu.txt.huff

# Decompress text
./compressor Testing/sonu.txt.huff
# → produces Testing/Uncomp_sonu.txt

# Compress an image
./compressor Testing/test.jpg
# → produces Testing/test.jpg.huff

# Decompress an image
./compressor Testing/test.jpg.huff
# → produces Testing/Uncomp_test.png
```

> On Windows, use `compressor.exe` instead of `./compressor`.

---

## 🌐 Use From Anywhere (Global CLI via PATH)

By adding the folder containing `compressor.exe` to your system's **PATH** environment variable, you can run it like any system command — from **any directory**.

### Without PATH vs With PATH

```
Without PATH:
  ./compressor file.txt              ← must be in same folder
  C:\tools\compressor file.txt       ← or use full path to exe

With PATH:
  compressor file.txt                ← works from ANYWHERE ✅
```

### 🪟 How to Add on Windows

1. Search **"Environment Variables"** in the Start menu
2. Click **"Edit the system environment variables"**
3. Click **"Environment Variables"**
4. Under **"System Variables"**, find and select **"Path"**
5. Click **"Edit"** → **"New"**
6. Add the folder where `compressor.exe` lives, e.g.:
   ```
   C:\Users\Sonu Kumar\Desktop\Programs\Compressor\
   ```
7. Click **OK** on all windows

Or use PowerShell (run as Administrator):
```powershell
[System.Environment]::SetEnvironmentVariable("Path", $env:Path + ";C:\Users\Sonu Kumar\Desktop\Programs\Compressor", "Machine")
```

### 🐧 How to Add on Linux / Mac

Add the following line to your `~/.bashrc` or `~/.zshrc`:

```bash
export PATH=$PATH:/path/to/Compressor
```

Then apply the changes:
```bash
source ~/.bashrc
```

### After PATH is set — works like any real CLI tool ✅

```bash
compressor file.txt                     # from any directory
compressor C:\Users\Sonu\notes.txt      # with full file path
```

Just like `git`, `python`, `gcc` — they all use PATH the same way!

---

## 📊 Compression Results

### ✅ Text File (Works Well)

| File                | Size                        |
|---------------------|-----------------------------|
| `sonu.txt`          | 1,048,584 bytes (~1 MB)     |
| `sonu.txt.huff`     | 630,924 bytes (~616 KB)     |
| `Uncomp_sonu.txt`   | 1,048,584 bytes (~1 MB)     |

> **Compression ratio: ~39.8% reduction** | Decoded file is byte-for-byte identical ✅

---

### ❌ Image File (Does NOT Compress Well)

| File                | Size                        |
|---------------------|-----------------------------|
| `test.jpg`          | 3,354,572 bytes (~3.2 MB)   |
| `test.jpg.huff`     | 63,881,547 bytes (~60.9 MB) |
| `Uncomp_test.png`   | ~equal to raw pixel size    |

> **The `.huff` file is ~19× LARGER than the original JPEG** — because JPEG is already compressed and the raw pixel data decoded from it is massive. See the [Image Compression Limitation](#️-image-compression-limitation) section above.

---

## 🧠 Algorithm Complexity

| Step                  | Time Complexity       |
|-----------------------|-----------------------|
| Frequency Counting    | O(n)                  |
| Huffman Tree Building | O(k log k), k ≤ 256  |
| Code Generation       | O(k)                  |
| Encoding              | O(n)                  |
| Decoding              | O(n)                  |

Where **n** = number of bytes in the file, **k** = number of unique byte values (max 256).

---

## 🔧 Implementation Details

- **Min-Heap via `std::priority_queue`**: Used with a custom comparator to always pop the lowest-frequency node.
- **Bit Packing**: Bits are packed 8-at-a-time into `unsigned char` buffers for efficient binary I/O.
- **Padding Handling**: The last byte's unused bits are tracked with a padding byte stored in the header to ensure perfect reconstruction.
- **Image I/O via STB**: Uses the single-header `stb_image.h` and `stb_image_write.h` libraries to load any image format and write PNG output.
- **Auto File Detection**: `main.cpp` checks the file extension to automatically route between text compression, image compression, and decompression.
- **Binary File Support**: All I/O runs in `ios::binary` mode for correct cross-platform byte handling.

---

## 📚 Concepts Used

- Greedy Algorithms
- Binary Trees & Tree Traversal
- Priority Queue (Min-Heap)
- Bitwise Operations
- File I/O (Binary mode)
- Image pixel-level encoding
- Modular C++ Programming (Headers + Source files)

---

## 👨‍💻 Author

**Sonu Kumar**  
Built as a Data Structures & Algorithms project in C++.

---

## 📄 License

This project is open-source and free to use for educational purposes.
