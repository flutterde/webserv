import os
import random

def create_random_files(base_path, num_files):
    """Create a random number of files in the specified directory."""
    for i in range(num_files):
        file_name = f"file_{i+1}.txt"
        file_path = os.path.join(base_path, file_name)
        with open(file_path, 'w') as f:
            f.write(f"This is the content of {file_name}")

def create_nested_structure(base_path, depth, max_folders, max_files):
    """Recursively create nested folders and files."""
    if depth <= 0:
        return
    
    # Create a random number of files in the current directory
    num_files = random.randint(1, max_files)
    create_random_files(base_path, num_files)
    
    # Create a random number of subfolders in the current directory
    num_folders = random.randint(1, max_folders)
    for i in range(num_folders):
        folder_name = f"folder_{i+1}"
        folder_path = os.path.join(base_path, folder_name)
        os.makedirs(folder_path, exist_ok=True)
        
        # Recursively create nested structure inside the subfolder
        create_nested_structure(folder_path, depth - 1, max_folders, max_files)

def main():
    # Define the base folder name
    base_folder = "myfld"
    
    # Create the base folder
    os.makedirs(base_folder, exist_ok=True)
    
    # Define parameters for the nested structure
    max_depth = 3  # Maximum depth of nested folders
    max_folders_per_level = 3  # Maximum number of folders per level
    max_files_per_level = 5  # Maximum number of files per level
    
    # Generate the nested folder and file structure
    create_nested_structure(base_folder, max_depth, max_folders_per_level, max_files_per_level)
    
    print(f"Folder structure created successfully in '{base_folder}'.")

if __name__ == "__main__":
    main()