#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

struct starData {
    int mass = -1;
    float x = -1;
    float y = -1;
};


struct quadTreeNode {
    starData nodeInfo;
    bool validData = false;
    int midX = -1;
    int midY = -1;
    quadTreeNode * nw = nullptr;
    bool nwChild = false;
    quadTreeNode * ne = nullptr;
    bool neChild = false;
    quadTreeNode * se = nullptr;
    bool seChild = false;
    quadTreeNode * sw = nullptr;
    bool swChild = false;
};

const int WIDTH = 1000;
const int HEIGHT = 1000;

void addStar(quadTreeNode * currNode, starData newStar) {
    //cout << newStar.x << ", " << newStar.y << endl;
    if (newStar.x < currNode-> midX)
    {
        if (newStar.y < currNode -> midY)
        {
            if (currNode -> nwChild)
            {
            	bool tmpValid = currNode -> nw -> validData;
            	starData tmpNode = currNode -> nw -> nodeInfo;
                addStar(currNode->nw, newStar);
                if (tmpValid)
				{
					addStar(currNode->nw, tmpNode);
				}
                currNode->nwChild = true;
                cout << "replaced existing node" << endl;
            }
            else
            {
                currNode->nw = new quadTreeNode;
                currNode->nw->nodeInfo = newStar;
                currNode->nw->midX = (currNode->midX / 2);
                currNode->nw->midY = (currNode->midY / 2);
                currNode->nwChild = true;
                cout << "added new node!" << endl;
                currNode ->nw->validData = true;
            }
        }
        else
        {
            if (currNode -> neChild)
            {
            	bool tmpValid = currNode -> ne -> validData;
            	starData tmpNode = currNode -> ne -> nodeInfo;
                addStar(currNode->ne, newStar);
                if (tmpValid)
				{
					addStar(currNode->ne, tmpNode);
				}
                currNode->neChild = true;
                cout << "replaced existing node" << endl;
            }
            else
            {
                currNode->ne = new quadTreeNode;
                currNode->ne->nodeInfo = newStar;
                currNode->ne->midX = (currNode->midX / 2);
                currNode->ne->midY = (currNode->midY / 2 + currNode->midY);
                currNode->neChild = true;
                cout << "added new node!" << endl;
                currNode ->ne->validData = true;
            }
        }
    }
    else
    {
        if (newStar.y < currNode -> midY)
        {
            if (currNode -> swChild)
            {
            	bool tmpValid = currNode -> sw -> validData;
            	starData tmpNode = currNode -> sw -> nodeInfo;
                addStar(currNode->sw, newStar);
                if (tmpValid)
				{
					addStar(currNode->sw, tmpNode);
				}
                currNode->swChild = true;
                cout << "replaced existing node" << endl;
            }
            else
            {
                currNode->sw = new quadTreeNode;
                currNode->sw->nodeInfo = newStar;
                currNode->sw->midX = (currNode->midX / 2 + currNode ->midX);
                currNode->sw->midY = (currNode->midY / 2);
                currNode->swChild = true;
                cout << "added new node!" << endl;
                currNode ->sw->validData = true;
            }
        }
        else
        {
            if (currNode -> seChild)
            {
            	bool tmpValid = currNode -> se -> validData;
            	starData tmpNode = currNode -> se -> nodeInfo;
                addStar(currNode->se, newStar);
                if (tmpValid)
				{
					addStar(currNode->se, tmpNode);
				}
                currNode->seChild = true;
                cout << "replaced existing node" << endl;
            }
            else
            {
                currNode->se = new quadTreeNode;
                currNode->se->nodeInfo = newStar;
                currNode->se->midX = (currNode->midX / 2 + currNode ->midX);
                currNode->se->midY = (currNode->midY / 2 + currNode->midY);
                currNode->seChild = true;
                cout << "added new node!" << endl;
                currNode ->validData = true;
            }
        }
    }
}


int main(){
    srand (time(NULL));
    quadTreeNode * mainNode = new quadTreeNode;
    mainNode->midX = WIDTH / 2;
    mainNode->midY = HEIGHT / 2;
    starData newStar;
    newStar.mass = 100;
    newStar.x = 122.584;
    newStar.y = 753.745;
    addStar(mainNode, newStar);
    for (int i = 0; i < 1000; i++)
    {
        starData generatedStar;
        generatedStar.mass = rand() % 1000 + 1;
        generatedStar.x = rand() % 1000;
        generatedStar.y = rand() % 1000;
        addStar(mainNode, generatedStar);
    }
    delete mainNode;

}
