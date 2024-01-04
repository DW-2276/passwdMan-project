#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <time.h>
#include <cstdio> // for overwriting file name rename(oldname, newname)
#define bitset(byte, nbit) ((byte) |= (1 << (nbit)))
#define bitclear(byte, nbit) ((byte) &= ~(1 << (nbit)))
#define bitflip(byte, nbit) ((byte) ^= (1 << (nbit)))
#define bitcheck(byte, nbit) ((byte) & (1 << (nbit)))

using namespace std;

class temList
{
public:
    temList(ifstream &in);                            // constructor
    void addL(int index, string n, string f, int pl); // make node in template list
    void listTem();
    void delTem(int ind);
    void genPswd(int ind);
    void genTem(string nm, string format, int tLen);
    void writeTem();
    void menue();

    int len;
    int inC; // index index

private:
    struct lnode
    {
        int index;
        string n;
        string format;
        int pLen;
        lnode *prev;
        lnode *next;
    };

    lnode *first;
    lnode *last;
};

temList::temList(ifstream &in)
{
    string f, na;
    int pl = 0;
    in >> len; // number of templates
    inC = len;
    first = NULL;
    last = NULL;
    for (int i = 0; i < len; i++)
    {
        in >> na;
        in >> f;
        in >> pl;
        // cout << "DEBUG:" << i << na << f << pl << endl;
        addL(i, na, f, pl);
    }
}

void temList::addL(int i, string na, string f, int pl)
{
    lnode *tmp;
    tmp = new lnode;
    tmp->index = i;
    tmp->n = na;
    tmp->format = f;
    tmp->pLen = pl;
    tmp->next = NULL;
    tmp->prev = last;
    if (last != NULL)
    {
        last->next = tmp;
        last = tmp;
    }
    else
    {
        first = tmp;
        last = tmp;
    }
}

void temList::listTem()
{
    if (len == 0)
    {
        cout << "There are no templates, make one" << endl;
        return;
    }
    lnode *walk;
    walk = first;
    cout << setw(9) << "    Index" << setw(20) << " Name" << setw(10) << "  Length" << setw(10) << "    Special Characters" << endl;
    while (walk != NULL)
    {
        cout << setw(9) << walk->index << setw(20) << walk->n << setw(10) << walk->pLen << setw(22) << walk->format << endl;
        walk = walk->next;
    }
    cout << endl;
}

void temList::genTem(string nm, string format, int tLen)
{

    ++len;
    ++inC;

    addL(inC - 1, nm, format, tLen);
}

void temList::genPswd(int ind)
{
    if (len <= 0)
        return;
    if (ind > inC)
        return;
    lnode *walk;
    lnode *tmp;
    walk = first;
    if (first == NULL)
        return;
    while (walk != NULL)
    {
        if (walk->index == ind)
        {
            tmp = walk;
        }
        walk = walk->next;
    }
    int l = tmp->pLen;

    string passwd;
    int rNum;
    // cout << "test pLen" << tmp->pLen << "  " << tmp->format.length() << endl; // crashes HERE
    srand(time(0));
    for (int i = 0; i < tmp->pLen; i++)
    {
        rNum = rand() % 7;
        // cout << rNum;
        if(rNum == 0 || rNum == 1) // a-z
            passwd += (rand() % (122 - 97 + 1)) + 97;
            // cout << "low" << endl;
        else if(rNum == 2 || rNum == 3) // A-Z
            passwd += (rand() % (90 - 65 + 1)) + 65;
            // cout << "high" << endl;
        else if(rNum == 4 || rNum == 5) // 0-9
            passwd += (rand() % (57 - 48 + 1)) + 48;
            // cout << "num" << endl;
        if(rNum == 6) // special char
            passwd += tmp->format.at(rand() % tmp->format.length());
            // cout << "spec" << endl;
    }
    cout << passwd << endl;
}

void temList::delTem(int ind)
{
    lnode *walk;
    lnode *tmp;
    tmp = NULL;
    walk = first;
    if (walk == NULL)
        return;
    if (walk->next == NULL)
    {
        first = NULL;
        --len;
        return;
    }
    if (first->index == ind)
    {
        first = first->next;
        --len;
        return;
    }
    while (walk != NULL)
    {
        if (walk->index == ind)
            tmp = walk;
        walk = walk->next;
    }
    if (tmp != NULL) // if something happened
    {
        walk = tmp->prev;
        walk->next = tmp->next;
        free(tmp);
        --len;
    }
}

void temList::writeTem()
{
    ofstream ofp;
    ofp.open("templates.txt");
    ofp << len << endl;
    //cout << "Writing " << len << " templates" << endl;
    lnode *walk;
    walk = first;
    if (first == NULL)
    {
        ofp.close();
        return;
    }
    else
    {
        while (walk != NULL)
        {
            ofp << walk->n << endl;
            ofp << walk->format << endl;
            ofp << walk->pLen << endl;
            walk = walk->next;
        }
        ofp.close();
    }
}

void temList::menue()
{
    int tIndex = -1;
    int menSelect = -1;
    string nm, form;
    int tLen;
    while (menSelect != 4)
    {
        cout << "Cyber Law & Ethics Final Project: Password Generator" << endl
             << endl
             << "0: Generate Password" << endl
             << "1: Generate Template" << endl
             << "2: Delete Template" << endl
             << "3: List Templates" << endl
             << "4: Exit" << endl;
        cin >> menSelect;
        switch (menSelect)
        {
        case 0:
            cout << "Enter the index of the template you want to use: ";
            cin >> tIndex;
            genPswd(tIndex);
            break;
        case 1:
            cout << "Template name (No spaces): ";
            cin >> nm;
            cout << "Length of password: ";
            cin >> tLen;

            cout << "Special Characters (!@#$%^&*): ";
            cin >> form;
            genTem(nm, form, tLen);
            break;
        case 2:
            cout << "Enter the index of the template you want to delete: ";
            cin >> tIndex;
            delTem(tIndex);
            break;
        case 3:
            listTem();
            break;
        case 4:
            writeTem();
            return;
            break;
        default:
            cout << "Please enter a valid selection" << endl;
        }
    }
}
int main(int argc, char **argv)
{
    ifstream ifp;
    ifp.open("templates.txt");
    if (!ifp.is_open())
    {
        cout << "templates.txt missing. Exiting" << endl;
        return 3;
    }
    temList a(ifp);
    ifp.close();

    int index, length;

    // ofstream ofp("tmpTemps.txt");

    if (argc < 2) // no args
        a.menue();
    else
    {
        switch (argv[1][0]) // cmd interface
        {
        case 'h':
            cout << "h                                  help function        " << endl
                 << "p <index>                          generates password" << endl
                 << "t <name> <!@#$%^&*> <length>       makes password template" << endl
                 << "d <index>                          deletes a template" << endl
                 << "l                                  lists available templates" << endl;
            break;
        case 'p':
            if (argc != 3)
                cerr << "invalid syntax: passGen p <index>" << endl;
            else
            {
                index = atoi(argv[2]);
                a.genPswd(index);
            }
            break;
        case 't':
            if (argc != 5)
                cerr << "Invalid syntax: passGen t <name> <!@#$%^&*> <length>" << endl;
            else
            {
                string name(argv[2]);
                string format(argv[3]);
                length = atoi(argv[4]);
                a.genTem(name, format, length);
                a.writeTem();
            }
            break;
        case 'd':
            if (argc != 3)
                cerr << "invalid syntax: passGen d <index>" << endl;
            else
            {
                index = atoi(argv[2]);
                a.delTem(index);
                a.writeTem();
            }
            break;
        case 'l':
            a.listTem();
            break;
        default:
            cerr << "Invalid arguments. See 'passGen h' for more info" << endl;
            return 2;
        }
    }

    return 0;
}