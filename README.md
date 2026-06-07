# 🗜️ Huffman File Compressor

A **lossless file compression tool** built in C++ using the **Huffman Encoding algorithm**. It compresses any binary/text file into a `.huff` format and can reconstruct the original file byte-for-byte.

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
├── main.cpp          # Entry point — orchestrates the full encode/decode pipeline
├── frequency.h       # Header for frequency counter
├── frequency.cpp     # Counts byte frequency from the input file
├── huffman.h         # Header for Huffman Node struct and tree builder
├── huffman.cpp       # Builds the Huffman tree using a min-heap priority queue
├── encoder.h         # Header for code generation and encoder
├── encoder.cpp       # Generates Huffman codes and encodes the file
├── decoder.h         # Header for the decoder
└── decoder.cpp       # Decodes .huff file and reconstructs original file
```

---

## ⚙️ How It Works

### Compression (Encoding)
1. The input file is read byte-by-byte and a **frequency table** (size 256) is built.
2. A **min-heap** is used to build the Huffman Tree — the two nodes with the lowest frequency are merged repeatedly until a single root remains.
3. The tree is traversed recursively to assign binary codes (`0` for left, `1` for right) to each byte.
4. The output `.huff` file is written with:
   - A **frequency table header** (256 × 4 bytes = 1024 bytes) for reconstruction
   - A **1-byte padding field** to handle non-multiple-of-8 bit lengths
   - **Packed bit data** (the compressed content)

### Decompression (Decoding)
1. The frequency table is read from the `.huff` file header.
2. The Huffman Tree is **reconstructed** from that table.
3. The compressed bits are traversed through the tree to recover each original byte.
4. The reconstructed file is saved as `Uncomp_<originalname>`.

---

## 🚀 Getting Started

### Prerequisites
- A C++ compiler (g++ recommended)
- C++11 or later

### Compilation

```bash
g++ main.cpp frequency.cpp huffman.cpp encoder.cpp decoder.cpp -o compressor
```

### Usage

```bash
./compressor <filename or full/path/to/file>
```

You can pass either a **filename** (if you're in the same folder) or a **full file path** from anywhere:

```bash
# Same folder
./compressor sonu.txt

# Full path to any file on your system
./compressor C:\Users\Sonu\Documents\report.txt
./compressor /home/user/documents/data.log
```

This will:
- Compress the file → produce `<file>.huff` alongside it
- Decompress `<file>.huff` → produce `Uncomp_<file>` in the same location

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

Tested on a sample text file:

| File              | Size         |
|-------------------|--------------|
| `sonu.txt`        | 1,048,580 bytes (~1 MB)  |
| `sonu.txt.huff`   | 630,920 bytes (~616 KB)  |
| `Uncomp_sonu.txt` | 1,048,580 bytes (~1 MB)  |

> **Compression ratio: ~39.8% reduction in file size**  
> The decoded file is an exact byte-for-byte match of the original ✅

---

## 🧠 Algorithm Complexity

| Step                  | Time Complexity       |
|-----------------------|-----------------------|
| Frequency Counting    | O(n)                  |
| Huffman Tree Building | O(k log k), k ≤ 256  |
| Code Generation       | O(k)                  |
| Encoding              | O(n)                  |
| Decoding              | O(n)                  |

Where **n** = number of bytes in the file, **k** = number of unique characters (max 256 for byte-level compression).

---

## 🔧 Implementation Details

- **Min-Heap via `std::priority_queue`**: Used with a custom comparator to always pop the lowest-frequency node.
- **Bit Packing**: Bits are packed 8-at-a-time into `unsigned char` buffers for efficient binary I/O.
- **Padding Handling**: The last byte's unused bits are tracked with a padding byte stored in the header to ensure perfect reconstruction.
- **Binary File Support**: The encoder/decoder work in `ios::binary` mode — they can compress any file type (text, images, etc.).

---

## 📚 Concepts Used

- Greedy Algorithms
- Binary Trees & Tree Traversal
- Priority Queue (Min-Heap)
- Bitwise Operations
- File I/O (Binary mode)
- Modular C++ Programming (Headers + Source files)

---

## 👨‍💻 Author

**Sonu Kumar**  
Built as a Data Structures & Algorithms project in C++.

---

## 📄 License

This project is open-source and free to use for educational purposes.
