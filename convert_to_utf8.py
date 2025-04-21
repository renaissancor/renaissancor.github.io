import os
import shutil
import chardet  # pip install chardet

TARGET_DIR = 'docs'
EXTENSIONS = ['.md', '.txt', '.csv']
BACKUP_DIR = 'encoding_backup'

def is_target_file(filename):
    return any(filename.lower().endswith(ext) for ext in EXTENSIONS)

def detect_encoding(filepath):
    with open(filepath, 'rb') as f:
        raw = f.read()
        result = chardet.detect(raw)
        return result['encoding'], raw

def convert_file(filepath, backup_root):
    encoding, raw_data = detect_encoding(filepath)
    
    if not encoding or encoding.lower().startswith('utf'):
        return False  # Already UTF or undetected

    # Backup first
    rel_path = os.path.relpath(filepath, TARGET_DIR)
    backup_path = os.path.join(backup_root, rel_path)
    os.makedirs(os.path.dirname(backup_path), exist_ok=True)
    shutil.copy2(filepath, backup_path)

    try:
        # Decode + split into consistent line endings
        decoded = raw_data.decode(encoding)
        # Normalize line endings to UNIX style (\n)
        decoded = decoded.replace('\r\n', '\n').replace('\r', '\n')
        with open(filepath, 'w', encoding='utf-8', newline='\n') as f:
            f.write(decoded)
        print(f"✅ Converted to UTF-8: {filepath} (was {encoding})")
        return True
    except Exception as e:
        print(f"❌ Failed to convert: {filepath} (detected {encoding}) — {e}")
        return False

def main():
    print("🔍 Scanning files for encoding issues...")
    total, converted = 0, 0

    for root, _, files in os.walk(TARGET_DIR):
        for file in files:
            if is_target_file(file):
                path = os.path.join(root, file)
                total += 1
                if convert_file(path, BACKUP_DIR):
                    converted += 1

    print(f"\n✅ Done. Scanned: {total} files | Converted: {converted} files.")
    print(f"💾 Backup saved in: {BACKUP_DIR}/")

if __name__ == '__main__':
    main()
