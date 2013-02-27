
How To Add A New File Format Loader\Saver

1:  Create the File Format Class
The file format class inherits from ImageIO (ImageIO/ImageIO.h) and contains the following
function:
  Load()                  Loads the image file and returns an Image instance
  Save()                  Saves the provided Image instance to a file

  IsRecognized()          Does a detailed scan of the file to see if it is recognized for loading
  IsRecognizedExtension() Does a compare against the extension of the filename to see if it is recognized for loading
  IsRecognizedHeader()    Does a compare against the 4-character header passed in to see if it is recognized for loading

  FillInImageData()       Fills in the provided ImageData class with details of the file just loaded
  FillInHeader()          Used to read information from an ImageData structure for saving a file.

  GetExtensionList()      Retuns a read-only poinyer to a NULL-terminated array of file extensions.

See the BMP File Format class for more information.

The above is all that is needed for a stand-alone file format.  However, if you want to use
the ImageData Smart Loader/Saver features, you must also do the following:

2: Add the new the File Format ID to ImageFileFormats (ImageIO/ImageIOEnums.h).  This ID is
used to determine what file format to was loaded and what format to save as.

3: Add the new File Format Name string to ImageFileFormatStrings (ImageIO/ImageIOStrings.h)  This
can be used in file requesters and similar to present information to the user.

4: Edit ImageData.cpp and add the loader code for the new file format to te Load() function.  This
mainly consists of copying/pasting another file format's code and changing it so it reference your
own functions.

5: Edit ImageData.cpp's Save() function in a similar manner.

That's it!

