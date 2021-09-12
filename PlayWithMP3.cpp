#include <iostream>
#include <fstream>  
#include <string> 
using namespace std;

struct TAG
{
	string Artist, Title, Album, Year, Comment;
};

int GetSize(FILE *file);
bool MP3Check(string filename);
string ReadChars(FILE *file, int currLoc);
string CheckHeader(FILE *file, int currLoc);
string Year(FILE *file, int currLoc);
void ReadMP3(FILE *file, TAG tag);
void WriteArtist(FILE *file, int filesize, char Artist[]);
void WriteTitle(FILE *file, int filesize, char Title[]);
void WriteAlbum(FILE *file, int filesize, char Album[]);
void WriteYear(FILE *file, int filesize, char Year[]);
void WriteComment(FILE *file, int filesize, char Comment[]);
void writeToFile(FILE *file, TAG tag, char newFile[]);

int main(int argc, char *argv[])  
{
	string fileName("");
	if(argc < 2)  
	{  
		cout<<"Drag and drop MP3 files to the Application!"<<endl;  //If not file dragged to exe warn user
		cin.get(); 
		return 0;  
	}  
	else
	{
		FILE *filePtr;
		for(int i=1; i<argc; i++)  
		{  
			cout<<"Opening File: "<< argv[i]<< endl;
			fileName = argv[i];
			filePtr = fopen(argv[i], "r+"); //Open a text file for update (that is, for both reading and writing).
		}

		if (filePtr) //If file exists
		{
			if (!MP3Check(fileName))
			{
				cout << "ERROR! File is not an MP3. Please use MP3 files only"<<endl;
				cin.get();
				fclose(filePtr);
				return 0;
			}
			else
			{
				cout << "File " <<fileName<<" Opened Successfully"<<endl;
				int currLoc = GetSize(filePtr)-128; //Go to byte location of information
				int filesize = GetSize(filePtr);//Store actual file size.
				string tag = CheckHeader(filePtr, currLoc); //Tag confirmation
				if (tag == "TAG")
				{
					cout<<"File Has A Valid MP3 Tag And Will Be Read Now"<<endl<<endl;
					currLoc+=3; //move past Tag location to next information
					TAG tag;
					ReadMP3(filePtr, tag);
					//At this point the user will be prompted with options to continue
					cout << "You May Now Edit the File's Tags" <<endl;
					cout << "1. Edit Artist"<<endl;
					cout << "2. Edit Title"<<endl;
					cout << "3. Edit Album"<<endl;
					cout << "4. Edit Year"<<endl;
					cout << "5. Edit Comment"<<endl;
					cout << "6. ReRead Info"<<endl;
					cout << "7. Write to new File"<<endl;
					cout << "8: Exit Program"<<endl<<endl;
					int choice;
					while (1)
					{
					cout << "Please Enter Your Choice And Then Press Enter: " ;
					cin>>choice;
					switch (choice)  //switch-case to set what happens on user input
						{
							case 1:
								char artist [31];
								cout << "Enter Desired Artist Name"<<endl;
								cin.ignore(); //sets break time for user to enter input
								cin.getline(artist, 31); //1 byte larger then needed to make up for enter key press
								WriteArtist(filePtr, filesize, artist); //calls write function
								break;
							case 2:
								char title[31];
								cout << "Enter Desired Title"<<endl;
								cin.ignore();
								cin.getline(title, 31);
								WriteTitle(filePtr, filesize, title); //same info as case 1
								break;
							case 3:
								char album[31];
								cout << "Enter Desired Album Title"<<endl;
								cin.ignore();
								cin.getline(album, 31);
								WriteAlbum(filePtr, filesize, album);//same info as case 1
								break;
							case 4:
								char year[5];
								cout << "Enter Desired Release Year"<<endl;
								cin.ignore();
								cin.getline(year, 5);
								WriteYear(filePtr, filesize, year); //same info as case 1
								break;
							case 5:
								char comment[31];
								cout << "Enter Desired Comments"<<endl;
								cin.ignore();
								cin.getline(comment, 31);
								WriteComment(filePtr, filesize, comment); //same info as case 1
								break;
							case 6:
								cout <<endl;
								ReadMP3(filePtr, tag); //If user enters 6 rereads mp3 info
								break;
							case 7:
							    char fileName[30];
							    cout<< "Enter name of the file to write"<<endl;
								cin.ignore();
								cin.getline(fileName, 11);
								writeToFile(filePtr, tag, fileName); //If user enters 6 rereads mp3 info
								break;
							case 8:
								fclose(filePtr);
								exit (0);
							default:
								cout << "The Number You Entered Is Not A Valid Option. Please Try Again" <<endl;
								break; //if not option gives error message
						}
					}
					cin.get();
					fclose(filePtr);//closes file and ends program
					return (0);
				}
				else
				{
					cout << "File Is Not Tagged With Information. Please Try A Different MP3"<<endl;
					cin.get();
					fclose(filePtr);// If file is not tagged gives error message, closes file and ends program
					return 0;
				}
			}
		}
		else
		{
			cout << "Error Opening File. Press The Enter Key To Exit" <<endl;
			cin.get(); //If opening error end program
			return 0;
		}
		cin.get();
	}
return(0);
}


int GetSize(FILE *file)
{
	//returns the size of the file as an int
	int curr = ftell(file);
	fseek(file,0,SEEK_END);
	int size = ftell(file);
	fseek(file, curr, SEEK_SET);
	return size;
}

bool MP3Check(string filename)
{
	//Checks if the file extension is mp3
	int filetype = filename.length() - 4;
	string filetipe = filename.substr(filetype, 4);
	if (filetipe == ".mp3") return true;
    return false;
}

string CheckHeader(FILE *file, int currLoc)
{
	//Checks for ID3v1 Tag CheckHeader 4bytes long...
	char sub[4];
	fseek(file , currLoc, SEEK_SET);
	fread(sub, 1, 4, file); 
	string str(sub);
	string ret = str.substr(0,3);
	return ret;
}
string Year(FILE *file, int currLoc)
{
	//Reads Year Of Track...4bytes long
	char sub[4];
	fseek(file , currLoc, SEEK_SET);
	fread(sub, 1, 4, file);
	string str(sub);
	return str;
}
string Read30Chars(FILE *file, int currLoc)
{
	//Used to read Title, Artist, Album, and Comment
	char sub[30];
	fseek(file , currLoc, SEEK_SET);
	fread(sub, 1, 30, file);
	string str(sub);
	return str;
}
void ReadMP3(FILE *file, TAG tag)
{
	//The following code reads the information for x bytes and then increases the currPtration x spaces
	//so the next information can be read
	int currLoc = GetSize(file)-125;
	tag.Title = Read30Chars(file, currLoc);
	currLoc+=30;
	tag.Artist = Read30Chars(file, currLoc);
	currLoc+=30;
	tag.Album = Read30Chars(file, currLoc);
	currLoc+=30;
	tag.Year = Year(file, currLoc);
	currLoc+=4;
	tag.Comment = Read30Chars(file, currLoc);
	cout << "Artist: " <<tag.Artist<<endl;
	cout << "Title: " <<tag.Title<<endl;
	cout << "Album: "<<tag.Album<<endl;
	cout << "Year: "<<tag.Year<<endl;
	cout << "Comment: "<<tag.Comment<<endl<<endl;
}
void writeToFile(FILE *file, TAG tag, char newFile[])
{
	fstream myfile;
	myfile.open(newFile,ios::out);
	if(!myfile)
    {
        cout<<"Error in creating file!!!";
        return;
    }

    int currLoc = GetSize(file)-125;
	tag.Title = Read30Chars(file, currLoc);
	currLoc+=30;
	tag.Artist = Read30Chars(file, currLoc);
	currLoc+=30;
	tag.Album = Read30Chars(file, currLoc);
	currLoc+=30;
	tag.Year = Year(file, currLoc);
	currLoc+=4;
	tag.Comment = Read30Chars(file, currLoc);
	myfile << "Artist: " <<tag.Artist<<endl;
	myfile << "Title: " <<tag.Title<<endl;
	myfile << "Album: "<<tag.Album<<endl;
	myfile << "Year: "<<tag.Year<<endl;
	myfile << "Comment: "<<tag.Comment<<endl<<endl;
}
void WriteArtist(FILE *file, int filesize, char Artist[])
{
	int writeLoc = filesize-95;
	fseek(file, writeLoc, SEEK_SET);
	fwrite(Artist, 1, 30, file); //goes to the storage of Artist data and writes new data
	cout << Artist << " set as The Artist"<<endl;
}
void WriteTitle(FILE *file, int filesize, char Title[])
{
	int writeLoc = filesize-125;
	fseek(file, writeLoc, SEEK_SET);
	fwrite(Title, 1, 30, file); //goes to the storage of Title data and writes new data
	cout << Title << " set as The Title"<<endl;
}
void WriteAlbum(FILE *file, int filesize, char Album[])
{
	int writeLoc = filesize-65;
	fseek(file, writeLoc, SEEK_SET);
	fwrite(Album, 1, 30, file); //goes to the storage of Album data and writes new data
	cout << Album << " set as The Album"<<endl;
}
void WriteYear(FILE *file, int filesize, char Year[])
{
	int writeLoc = filesize-35;
	fseek(file, writeLoc, SEEK_SET);
	fwrite(Year, 1, 4, file); //goes to the storage of Year data and writes new data
	cout << Year << " set as The Release Year"<<endl;
}
void WriteComment(FILE *file, int filesize, char Comment[])
{
	int writeLoc = filesize-31;
	fseek(file, writeLoc, SEEK_SET);
	fwrite(Comment, 1, 30, file); //goes to the storage of Comment data and writes new data
	cout << Comment << " set as The Comment"<<endl;
}