#include <iostream>
#include <string>
#include <sstream>
#include <dirent.h>   // For directory operations
#include <sys/stat.h> // Include for mkdir and rmdir
#include <errno.h>
#include <cstring>
#include <fstream>   // Required for file creation
#include <unistd.h>  // For chdir, getcwd, link, unlink, symlink
#include <fcntl.h>   // For open, read, write, close
#include <utime.h>   // For utime
#include <sys/types.h> //off_t

using namespace std;

// Function to move a file
void MoveFile(string sourcePath, string destinationDirectory) {
    cout << "Program successfully routed to MoveFile" << endl;

    // Construct the full destination path
    string destinationPath = destinationDirectory + "/" + sourcePath.substr(sourcePath.find_last_of("/") + 1);

    // Use rename() to move the file
    if (rename(sourcePath.c_str(), destinationPath.c_str()) == 0) {
        cout << "File moved successfully from '" << sourcePath << "' to '" << destinationPath << "'" << endl;
    } else {
        cerr << "Error moving file from '" << sourcePath << "' to '" << destinationPath << "': " << strerror(errno) << endl;
    }
}

// Function to list files in a directory
void listFiles(string path) {
    cout << "Program successfully routed to List files function..." << endl;
    cout << "Listing all the files under " + path << endl;
    DIR *dir;
    struct dirent *ent;
    dir = opendir(path.c_str());
    if (dir != NULL) {
        cout << "Files and directories in '" << path << "':" << endl;
        while ((ent = readdir(dir)) != NULL) {
            // Print the name of the file or directory
            cout << ent->d_name << endl;
        }
        closedir(dir);
    } else {
        // Handle the error, print to cerr
        cerr << "Error opening directory '" << path << "': " << strerror(errno) << endl;
    }
}

// Function to copy a file
void copyFile(string sourcePath, string destinationPath) {
    ifstream sourceFile(sourcePath, ios::binary);
    ofstream destFile(destinationPath, ios::binary);

    if (!sourceFile.is_open()) {
        cerr << "Error opening source file: " << sourcePath << endl;
        return;
    }
    if (!destFile.is_open()) {
        cerr << "Error opening destination file: " << destinationPath << endl;
        return;
    }

    destFile << sourceFile.rdbuf(); // Copy the entire file content

    if (sourceFile.fail())
        cerr << "Error reading source file" << endl;
    if (destFile.fail())
        cerr << "Error writing destination file" << endl;

    sourceFile.close();
    destFile.close();
    cout << "File copied successfully" << endl;
}

// Function to rename a file
void renameFile(string oldPath, string newPath) {
    if (rename(oldPath.c_str(), newPath.c_str()) == 0) {
        cout << "File renamed successfully from '" << oldPath << "' to '" << newPath << "'" << endl;
    } else {
        cerr << "Error renaming file from '" << oldPath << "' to '" << newPath << "': " << strerror(errno) << endl;
    }
}

// Function to delete a file
void deleteFileFunc(string filePath) {
    if (remove(filePath.c_str()) == 0) {
        cout << "File '" << filePath << "' deleted successfully." << endl;
    } else {
        cerr << "Error deleting file '" << filePath << "': " << strerror(errno) << endl;
    }
}

// Function to create a file
void createFile(string filePath) {
    ofstream newFile(filePath);
    if (newFile.is_open()) {
        cout << "File '" << filePath << "' created successfully." << endl;
        newFile.close();
    } else {
        cerr << "Error creating file '" << filePath << "'" << endl;
    }
}

// Function to delete a directory
void deleteDirectoryFunc(string dirPath) {
    if (rmdir(dirPath.c_str()) == 0) {
        cout << "Directory '" << dirPath << "' deleted successfully." << endl;
    } else {
        cerr << "Error deleting directory '" << dirPath << "': " << strerror(errno) << endl;
    }
}

// Function to create a directory
void createDirectory(string dirPath) {
    if (mkdir(dirPath.c_str(), 0777) == 0) { // 0777 for full permissions on Unix-like
        cout << "Directory '" << dirPath << "' created successfully." << endl;
    } else {
        cerr << "Error creating directory '" << dirPath << "': " << strerror(errno) << endl;
    }
}

// Function to change the current working directory
void changeDirectory(string path) {
    if (chdir(path.c_str()) == 0) {
        cout << "Changed directory to '" << path << "'" << endl;
    } else {
        cerr << "Error changing directory to '" << path << "': " << strerror(errno) << endl;
    }
}

// Function to get the current working directory
void printWorkingDirectory() {
    char buffer[1024];
    if (getcwd(buffer, sizeof(buffer)) != NULL) {
        cout << "Current working directory: " << buffer << endl;
    } else {
        cerr << "Error getting current working directory: " << strerror(errno) << endl;
    }
}

// Function to create a hard link
void createHardLink(string sourcePath, string linkPath) {
    if (link(sourcePath.c_str(), linkPath.c_str()) == 0) {
        cout << "Hard link created successfully from '" << sourcePath << "' to '" << linkPath << "'" << endl;
    } else {
        cerr << "Error creating hard link from '" << sourcePath << "' to '" << linkPath << "': " << strerror(errno) << endl;
    }
}

// Function to create a symbolic link
void createSymbolicLink(string sourcePath, string linkPath) {
    if (symlink(sourcePath.c_str(), linkPath.c_str()) == 0) {
        cout << "Symbolic link created successfully from '" << sourcePath << "' to '" << linkPath << "'" << endl;
    } else {
        cerr << "Error creating symbolic link from '" << sourcePath << "' to '" << linkPath << "': " << strerror(errno) << endl;
    }
}

// Function to read link
void readLinkFunc(string linkPath) {
    char buffer[1024];
    ssize_t bytesRead = readlink(linkPath.c_str(), buffer, sizeof(buffer) - 1); //readlink does not null terminate
    if (bytesRead != -1) {
        buffer[bytesRead] = '\0'; // Null-terminate the buffer
        cout << "Link '" << linkPath << "' points to '" << buffer << "'" << endl;
    } else {
        cerr << "Error reading link '" << linkPath << "': " << strerror(errno) << endl;
    }
}

// Function to change file permissions (chmod)
void changeFilePermissions(string filePath, mode_t mode) {
    if (chmod(filePath.c_str(), mode) == 0) {
        cout << "Permissions of file '" << filePath << "' changed successfully." << endl;
    } else {
        cerr << "Error changing permissions of file '" << filePath << "': " << strerror(errno) << endl;
    }
}

// Function to get file information (stat)
void getFileInfo(string filePath) {
    struct stat fileStat;
    if (stat(filePath.c_str(), &fileStat) == 0) {
        cout << "File information for '" << filePath << "':" << endl;
        cout << "  Size: " << fileStat.st_size << " bytes" << endl;
        cout << "  Inode: " << fileStat.st_ino << endl;
        cout << "  Permissions: " << fileStat.st_mode << endl;
        cout << "  Last modified: " << ctime(&fileStat.st_mtime); // ctime adds a newline
    } else {
        cerr << "Error getting file information for '" << filePath << "': " << strerror(errno) << endl;
    }
}

// Function to truncate a file
void truncateFile(string filePath, off_t length) {
    if (truncate(filePath.c_str(), length) == 0) {
        cout << "File '" << filePath << "' truncated to " << length << " bytes." << endl;
    } else {
        cerr << "Error truncating file '" << filePath << "': " << strerror(errno) << endl;
    }
}

// Function to get file descriptor
void getFileDescriptor(string filePath) {
    int fd = open(filePath.c_str(), O_RDONLY); // Example: Open for reading
    if (fd != -1) {
        cout << "File descriptor for '" << filePath << "' is " << fd << endl;
        close(fd); // Remember to close the file descriptor
    } else {
        cerr << "Error getting file descriptor for '" << filePath << "': " << strerror(errno) << endl;
    }
}

// Function to change file access and modification times (utime)
void changeFileTimes(string filePath, time_t accessTime, time_t modificationTime) {
    struct utimbuf times;
    times.actime = accessTime;
    times.modtime = modificationTime;
    if (utime(filePath.c_str(), &times) == 0) {
        cout << "File times for '" << filePath << "' changed successfully." << endl;
    } else {
        cerr << "Error changing file times for '" << filePath << "': " << strerror(errno) << endl;
    }
}

// Function to read from a file using a file descriptor
void readFileDescriptor(int fd, size_t bufferSize) {
    char buffer[bufferSize];
    ssize_t bytesRead = read(fd, buffer, bufferSize);
    if (bytesRead > 0) {
        cout.write(buffer, bytesRead);
    } else if (bytesRead == 0) {
        cout << "End of file reached." << endl;
    } else {
        cerr << "Error reading from file descriptor " << fd << ": " << strerror(errno) << endl;
    }
}

// Function to append content to a file
void appendToFile(string filePath, string content) {
    ofstream file(filePath, ios::app);
    if (file.is_open()) {
        file << content << endl;
        cout << "Content appended to '" << filePath << "' successfully." << endl;
        file.close();
    } else {
        cerr << "Error opening file '" << filePath << "': " << strerror(errno) << endl;
    }
}

// Function to change file ownership
void changeFileOwnership(string filePath, uid_t uid, gid_t gid) {
    if (chown(filePath.c_str(), uid, gid) == 0) {
        cout << "Ownership of file '" << filePath << "' changed successfully." << endl;
    } else {
        cerr << "Error changing ownership of file '" << filePath << "': " << strerror(errno) << endl;
    }
}


int main() {
    int ch;

    while (ch != 22) {
        cout << "-----------------Welcome to ZimOS---------------" << endl << endl;
        cout << "Enter 1 to list all files in a given directory" << endl;
        cout << "Enter 2 to move a file from one directory to another" << endl;
        cout << "Enter 3 to copy a file from one directory to another" << endl;
        cout << "Enter 4 to rename a file (Please give absolute paths only)" << endl;
        cout << "Enter 5 to delete a file" << endl;
        cout << "Enter 6 to create a file" << endl;
        cout << "Enter 7 to create a directory" << endl;
        cout << "Enter 8 to delete the whole directory" << endl;
        cout << "Enter 9 to append content to a file" << endl;
        cout << "Enter 10 to change the current directory" << endl;
        cout << "Enter 11 to print the current working directory" << endl;
        cout << "Enter 12 to create a hard link" << endl;
        cout << "Enter 13 to create a symbolic link" << endl;
        cout << "Enter 14 to read a symbolic link" << endl;
        cout << "Enter 15 to change file permissions (chmod)" << endl;
        cout << "Enter 16 to get file information (stat)" << endl;
        cout << "Enter 17 to truncate a file" << endl;
        cout << "Enter 18 to get file descriptor" << endl;
        cout << "Enter 19 to change file access and modification times (utime)" << endl;
        cout << "Enter 20 to read from file using file descriptor" << endl;
        cout << "Enter 21 to change file ownership" << endl;
        cout << "Enter 22 to exit" << endl;
        cout << "Enter your choice below" << endl;
        cin >> ch;
        cin.ignore(); // Consume the newline character after reading the number.

        string Lis_Dir_Name;
        string source_file_path;
        string dest_file_path;
        string new_file_path;
        string old_file_path;
        string del_file_path;
        string create_file_path;
        string del_dir_path;
        string change_dir_path;
        string hard_link_source;
        string hard_link_path;
        string sym_link_source;
        string sym_link_path;
        string read_link_path;
        string chmod_file_path;
        int chmod_mode;
        string stat_file_path;
        string truncate_file_path;
        off_t truncate_length;
        string fd_file_path;
        string utime_file_path;
        time_t access_time;
        time_t modification_time;
        int read_fd;
        size_t read_buffer_size;
        string chown_file_path;
        string content;
        uid_t uid; 
        gid_t gid; 

        switch (ch) {
            case 1:
                cout << "Enter the directory for which all the files have to be listed: ";
                getline(cin, Lis_Dir_Name);
                listFiles(Lis_Dir_Name);
                break;
            case 2:
                cout << "Enter the absolute path of the file you would like to move: ";
                getline(cin, source_file_path);
                cout << "Enter the absolute destination path (directory) where the file has to be moved: ";
                getline(cin, dest_file_path);
                MoveFile(source_file_path, dest_file_path);
                break;
            case 3:
                cout << "Enter the absolute path of the source file: ";
                getline(cin, source_file_path);
                cout << "Enter the absolute path of the destination file: ";
                getline(cin, dest_file_path);
                copyFile(source_file_path, dest_file_path);
                break;
            case 4:
                cout << "Enter the absolute path of the file to be renamed: ";
                getline(cin, old_file_path);
                cout << "Enter the new absolute path of the file: ";
                getline(cin, new_file_path);
                renameFile(old_file_path, new_file_path);
                break;
            case 5:
                cout << "Enter the absolute path of the file to be deleted: ";
                getline(cin, del_file_path);
                deleteFileFunc(del_file_path);
                break;
            case 6:
                cout << "Enter the absolute path of the file to be created: ";
                getline(cin, create_file_path);
                createFile(create_file_path);
                break;
            case 7:
                cout << "Enter the absolute path of the directory to be created: ";
                getline(cin, create_file_path);
                createDirectory(create_file_path);
                break;
            case 8:
                cout << "Enter the absolute path of the directory to be deleted: ";
                getline(cin, del_dir_path);
                deleteDirectoryFunc(del_dir_path);
                break;
            case 9:
                cout << "Enter the absolute path of the file to append to: ";
                getline(cin, create_file_path);
                cout << "Enter the content to append: ";
                getline(cin, content);
                appendToFile(create_file_path, content);
                break;
            case 10:
                cout << "Enter the path to change the current directory to: ";
                getline(cin, change_dir_path);
                changeDirectory(change_dir_path);
                break;
            case 11:
                printWorkingDirectory();
                break;
            case 12:
                cout << "Enter the absolute path of the source file for the hard link: ";
                getline(cin, hard_link_source);
                cout << "Enter the absolute path of the hard link: ";
                getline(cin, hard_link_path);
                createHardLink(hard_link_source, hard_link_path);
                break;
            case 13:
                cout << "Enter the absolute path of the source file for the symbolic link: ";
                getline(cin, sym_link_source);
                cout << "Enter the absolute path of the symbolic link: ";
                getline(cin, sym_link_path);
                createSymbolicLink(sym_link_source, sym_link_path);
                break;
            case 14:
                cout << "Enter the absolute path of the symbolic link to read: ";
                getline(cin, read_link_path);
                readLinkFunc(read_link_path);
                break;
            case 15:
                cout << "Enter the absolute path of the file to change permissions: ";
                getline(cin, chmod_file_path);
                cout << "Enter the new permissions in octal (e.g., 0777): ";
                cin >> oct >> chmod_mode;
                cin.ignore(); // Consume the newline
                changeFilePermissions(chmod_file_path, chmod_mode);
                break;
            case 16:
                cout << "Enter the absolute path of the file to get information: ";
                getline(cin, stat_file_path);
                getFileInfo(stat_file_path);
                break;
            case 17:
                cout << "Enter the absolute path of the file to truncate: ";
                getline(cin, truncate_file_path);
                cout << "Enter the length to truncate the file to: ";
                cin >> truncate_length;
                cin.ignore();
                truncateFile(truncate_file_path, truncate_length);
                break;
            case 18:
                cout << "Enter the absolute path of the file to get the file descriptor: ";
                getline(cin, fd_file_path);
                getFileDescriptor(fd_file_path);
                break;
            case 19:
                cout << "Enter the absolute path of the file to change times: ";
                getline(cin, utime_file_path);
                cout << "Enter the access time (seconds since epoch): ";
                cin >> access_time;
                cout << "Enter the modification time (seconds since epoch): ";
                cin >> modification_time;
                cin.ignore();
                changeFileTimes(utime_file_path, access_time, modification_time);
                break;
            case 20:
                cout << "Enter the file descriptor: ";
                cin >> read_fd;
                cout << "Enter the buffer size: ";
                cin >> read_buffer_size;
                cin.ignore();
                readFileDescriptor(read_fd, read_buffer_size);
                break;
            case 21:
                cout << "Enter the absolute path of the file to change ownership: ";
                getline(cin, chown_file_path);
                cout << "Enter the new UID: ";
                cin >> uid;
                cout << "Enter the new GID: ";
                cin >> gid;
                cin.ignore();
                changeFileOwnership(chown_file_path, uid, gid);
                break;
            case 22:
                cout << "Exiting program." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }
    return 0;
}

