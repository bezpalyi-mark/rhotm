//
// Created by hunterlan on 6/12/19.
//

#include "Functions.h"

void ResizeImage(string pathToImage, string pathToSave)
{
    Image image(pathToImage);

    Geometry sizeImage;
    sizeImage = image.size();

    if(sizeImage.height() > HEIGHT && sizeImage.width() > WIDTH)
    {
        Geometry geometry;
        geometry.aspect(true);
        geometry.width(WIDTH);
        geometry.height(HEIGHT);

        image.resize(geometry);
        if(pathToSave.size() == 0)
            pathToSave = pathToImage;
        if(!fs::exists(pathToSave))
        {
            fs::create_directory(pathToSave);
        }
        image.write(pathToSave);
    }
}

void PoolThreading(GtkButton * button, MainWindow * getWindow)
{
    vector<string> listOfImages;
    string pathToImages = gtk_entry_get_text((GtkEntry *)getWindow->entryPath);
    string saveImages = gtk_entry_get_text((GtkEntry *)getWindow->entrySave);
    if(pathToImages.size() == 0)
    {
        //MessageBox
    }
    else if(!fs::exists(pathToImages))
    {
        //MessageBox
    }
    else
    {
        for (const auto & entry : fs::directory_iterator(pathToImages))
        {
            if(entry.path() == pathToImages)
                continue;
            listOfImages.push_back(entry.path());
        }
        vector<thread> threads;
        int countToWork = listOfImages.size();
        int index = 0;
        while(countToWork > -COUNT_THREADS)
        {
            for(int i = 0; i < COUNT_THREADS; i++)
            {
                thread threader(ResizeImage, listOfImages[index], saveImages);
                if(i == COUNT_THREADS - 1 && threader.joinable())
                    threader.join();
                else
                    threader.detach();
                //threads.push_back(move(threader));
                index++;
            }
            /*
            for(int i = 0; i < threads.size(); i++)
            {
                if(i == threads.size() - 1 && threads[i].joinable())
                    threads[i].join();
                else
                    threads[i].detach();
            }*/

            countToWork-=COUNT_THREADS;
        }
    }
}