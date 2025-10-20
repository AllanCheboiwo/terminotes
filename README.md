# terminotes

A simple, lightweight terminal-based note-taking application written in C. Terminotes allows you to quickly capture and retrieve notes from the command line using a SQLite database for persistent storage.

## Features

- **Quick note capture**: Easily add notes with a simple command
- **Time-based filtering**: View notes from specific timeframes (last 24 hours, week, month, or all)
- **Persistent storage**: All notes are saved to a SQLite database
- **Timestamps**: Each note is automatically timestamped when created
- **Simple CLI interface**: Easy-to-use command structure

## Usage

### Add a Note
```bash
./notesapp take "Your note here"
```

### Read Notes

Read notes from the last week (default):
```bash
./notesapp read
```

Read notes from the last 24 hours:
```bash
./notesapp read -d
```

Read notes from the last week:
```bash
./notesapp read -w
```

Read notes from the last month:
```bash
./notesapp read -m
```

Read all notes ever created:
```bash
./notesapp read -a
```

### Get Help
```bash
./notesapp help
```

## Examples

```bash
# Add a meeting note
./notesapp take "Meeting with team at 3 PM"

# View today's notes
./notesapp read -d

# View all notes from the past month
./notesapp read -m
```

## Technical Details

- **Language**: C
- **Database**: SQLite3
- **Data Storage**: `notes.db` file in the application directory
- **Build**: Compile with `gcc` linking against SQLite3 library
