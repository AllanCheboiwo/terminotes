# terminotes

A simple, lightweight terminal-based note-taking application written in C. Terminotes allows you to quickly capture and retrieve notes from the command line using a SQLite database for persistent storage.

## Features

- **Quick note capture**: Easily add notes with a simple command
- **Time-based filtering**: View notes from specific timeframes (last 24 hours, week, month, or all)
- **Persistent storage**: All notes are saved to a SQLite database
- **Timestamps**: Each note is automatically timestamped when created
- **Simple CLI interface**: Easy-to-use command structure
- **System-wide access**: Run from any directory after installation

## Prerequisites

Before building, ensure you have:
- **GCC** - C compiler
- **SQLite3 development headers** - For compiling against SQLite3

### Install Prerequisites

**macOS:**
```bash
brew install sqlite3
```

**Ubuntu/Debian:**
```bash
sudo apt-get install build-essential sqlite3 libsqlite3-dev
```

**Fedora/RHEL:**
```bash
sudo dnf install gcc sqlite-devel
```

## Installation

1. Clone the repository:
```bash
git clone <repository-url>
cd terminotes
```

2. Build the application:
```bash
make
```

3. Install system-wide:
```bash
make install
```

After installation, you can run `terminotes` from any terminal directory.

## Usage

### Add a Note
```bash
terminotes take "Your note here"
```

### Read Notes

Read notes from the last week (default):
```bash
terminotes read
```

Read notes from the last 24 hours:
```bash
terminotes read -d
```

Read notes from the last week:
```bash
terminotes read -w
```

Read notes from the last month:
```bash
terminotes read -m
```

Read all notes ever created:
```bash
terminotes read -a
```

### Get Help
```bash
terminotes help
```

## Examples

```bash
# Add a meeting note
terminotes take "Meeting with team at 3 PM"

# View today's notes
terminotes read -d

# View all notes from the past month
terminotes read -m
```

## Uninstallation

To remove terminotes from your system:
```bash
make uninstall
make clean
```

## Technical Details

- **Language**: C
- **Database**: SQLite3
- **Data Storage**: `~/.config/terminotes/notes.db` 
- **Build**: Standard Makefile with `make`, `make install`, `make clean`, `make uninstall` targets
