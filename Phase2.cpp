#include <bits/stdc++.h>
using namespace std;

//* For File Handling
fstream finput, foutput;
//* For Memory
vector<vector<char>> memory(300, vector<char>(4, '_'));
//*
vector<char> R(4), IR(4);
//*
int TTL = 0, TLL = 0, JID = 0, IC = 0, C = 0, PTR = 0, k;
//* Interrupts
int PI, SI = 3, TI = 0;
//*
int pageTable[30];
//* To printing the no of jobs
int noOfJobs = 0;

struct PCB
{
    int TTC, LLC, TTL, TLL, JID;
};
//* Jobs
PCB jobs[10];

void INIT(string);
void INIT_PCB(string);
int ALLOCATE();

void MOS();
void READ();
void WRITE();
void TERMINATE(int);
void LOAD();
void START();
void EXECUTE_USER_PROGRAM();
int ADDRESS_MAP(int);
void MEMORY();
void PAGE_TABLE_SET(int);

int main()
{

    LOAD();

    return 0;
}

void LOAD()
{

    string line;

    finput.open("input.txt", ios::in);
    foutput.open("output.txt", ios::out);

    if (!finput)
    {
        cout << "No such file" << endl;
        return;
    }

    while (getline(finput, line))
    {
        // cout << line << endl;
        if (line.substr(0, 4) == "$AMJ")
        {

            cout << "\nInside AMJ\n";
            ++noOfJobs;
            PI = 0;
            INIT(line);
            PTR = ALLOCATE() * 10;
            //* Set Page Table fn. Put * in every byte. Indicating no page table entry is made
            cout << "PTR = " << PTR << endl;
            PAGE_TABLE_SET(PTR);
            k = PTR;
        }
        else if (line.substr(0, 4) == "$DTA")
        {
            cout << "\nInside DTA\n";
            START();
        }
        else if (line.substr(0, 4) == "$END")
        {
            cout << "\nInside END\n";
            MEMORY();
            cout << endl
                 << endl;
        }
        else
        {
            cout << "\nInside Else Condition\n";
            int index = ALLOCATE();
            cout << "Index = " << index << endl;
            memory[k][0] = '_';
            memory[k][1] = '_';
            memory[k][2] = index / 10 + '0';
            memory[k][3] = index % 10 + '0';
            index *= 10;
            int j = 0;
            while (j < 40 && j < line.length())
            {
                cout << "M[" << index << "]: ";
                for (int i = 0; i < 4 && j < line.length(); ++i)
                {
                    memory[index][i] = line[j++];
                    cout << memory[index][i] << " ";
                }
                ++index;
                cout << endl;
            }
            ++k;
        }
    }

    cout << "No of Jobs = " << noOfJobs << endl;

    finput.close();
    foutput.close();
}

void START()
{
    cout << "\nInside Start\n";
    IC = 0;
    EXECUTE_USER_PROGRAM();
}

void MEMORY()
{
    cout << "\nIn Memory\n";
    for (int i = 0; i < 300; ++i)
    {
        if (i % 10 == 0)
        {
            cout << i << "\n";
        }
        for (int j = 0; j < 4; ++j)
        {
            cout << memory[i][j] << " ";
        }
        cout << "\n";
    }
}

void PAGE_TABLE_SET(int PTR)
{
    cout << "\nIn PAGE TABLE SET\n";
    for (int i = PTR; i < PTR + 10; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            memory[i][j] = '$';
        }
    }
}

int ADDRESS_MAP(int VA)
{
    //* Handle if VA is not between 0 and 99. Then PI=2 Operand Error
    cout << "Inside ADMAP\n";
    cout << "PTR : " << PTR << ", VA : " << VA << endl;
    int PTE = PTR + (VA / 10);
    cout << "PTE : " << PTE << endl;
    char temp[4];
    for (int i = 0; i < 4; ++i)
    {
        cout << memory[PTE][i];
        temp[i] = memory[PTE][i];
    }
    int realAddress = 30;

    if (temp[2] == '0' || temp[2] == '1' || temp[2] == '2')
    {
        realAddress = ((int)temp[2] - 48) * 10 + ((int)temp[3] - 48);
    }
    if (realAddress >= 0 && realAddress < 30)
    {
        realAddress = realAddress * 10;
        realAddress = realAddress + (VA) % 10;
    }
    else
    {
        PI = 3;
    }
    cout << "\nRA:" << realAddress;
    cout << "\nPI:" << PI;
    return (realAddress);
}

void EXECUTE_USER_PROGRAM()
{
    cout << "Inside EXECUTE_USER_PROGRAM\n";

    int realAddress = ADDRESS_MAP(IC);

    if (PI == 0)
    {
        IR.clear();
        IR.push_back(memory[realAddress][0]);
        IR.push_back(memory[realAddress][1]);
        IR.push_back(memory[realAddress][2]);
        IR.push_back(memory[realAddress][3]);
        if (memory[realAddress][1] != '\n' && memory[realAddress][1] != '$' && IR[0] != 'H')
        {
            cout << "\nIR = " << string(IR.begin(), IR.end()) << endl;
            ++IC;
            int tensPlace = (int)IR[2] - 48;
            int onesPlace = (int)IR[3] - 48;
            if (tensPlace < 10 && onesPlace < 10 && tensPlace >= 0 && onesPlace >= 0)
            {
                tensPlace = (tensPlace * 10) + onesPlace;
                realAddress = ADDRESS_MAP(tensPlace);
            }
            else
            {
                PI = 2;
            }
            if (PI == 0)
            {
                if (IR[0] == 'L' && IR[1] == 'R')
                {
                    cout << "\nIn LR\n";
                    for (int i = 0; i < 4; ++i)
                    {
                        R[i] = memory[realAddress][i];
                    }
                }
                else if (IR[0] == 'S' && IR[1] == 'R')
                {
                    cout << "\nIn SR\n";
                    for (int i = 0; i < 4; ++i)
                    {
                        memory[realAddress][i] = R[i];
                    }
                }
                else if (IR[0] == 'C' && IR[1] == 'R')
                {
                    cout << "\nIn CR\n";
                    bool equal = true;
                    for (int i = 0; i < 4; ++i)
                    {
                        if (memory[realAddress][i] != R[i])
                        {
                            equal = false;
                            break;
                        }
                    }
                    if (equal)
                    {
                        C = 1;
                    }
                    else
                    {
                        C = 0;
                    }
                }
                else if (IR[0] == 'B' && IR[1] == 'T')
                {
                    cout << "\nIn BT\n";
                    if (C == 1)
                    {
                        IC = ((int)IR[2] - 48) * 10 + ((int)IR[3] - 48);
                    }
                }
                else if (IR[0] == 'G' && IR[1] == 'D')
                {
                    cout << "\nIn GD\n";
                    SI = 1;
                }
                else if (IR[0] == 'P' && IR[1] == 'D')
                {
                    cout << "\nIn PD\n";
                    SI = 2;
                }
                else
                {
                    PI = 1;
                }
            }
        }
        else if (IR[0] == 'H' && IR[1] == '_')
        {
            cout << "\nIn H\n";
            cout << IR[0] << " " << IR[1] << endl;
            ++IC;
            SI = 3;
        }
        else
        {
            cout << IR[0] << " " << IR[1] << endl;
            PI = 1;
        }
    }
    ++jobs[noOfJobs].TTC;
    cout << "\njobs[noOfJobs].TTC = " << jobs[noOfJobs].TTC << endl;
    if (jobs[noOfJobs].TTC >= jobs[noOfJobs].TTL)
    {
        TI = 2;
    }
    if (TI != 0 || PI != 0 || SI != 0)
    {
        cout << "\nGoing to MOS function\n";
        cout << "TI - " << TI << ", PI - " << PI << ", SI - " << SI << endl;
        MOS();
    }
    else
    {
        cout << "\nNot going to MOS function\n";
        EXECUTE_USER_PROGRAM();
    }
}

void INIT_PCB(string line)
{
    cout << "\nInside INIT_PCB\n";
    //* To get JID, TTL and TLL
    JID = stoi(line.substr(4, 4));
    TTL = stoi(line.substr(8, 4));
    TLL = stoi(line.substr(12, 4));

    //* Initialize PCB
    PCB currentJob;
    currentJob.JID = JID;
    currentJob.TTL = TTL;
    currentJob.TLL = TLL;
    currentJob.TTC = 0;
    currentJob.LLC = 0;
    jobs[noOfJobs] = currentJob;
}

void INIT(string line)
{
    cout << "\nInside INIT\n";
    INIT_PCB(line);
    // Memory Initialized by '_'
    for (int i = 0; i < memory.size(); ++i)
    {
        for (int j = 0; j < memory[i].size(); ++j)
        {
            memory[i][j] = '_';
        }
    }

    R.clear();
    PTR = 0;
    IC = 0;
    SI = 0;
    PI = 0;
    TI = 0;
    for (int i = 0; i < 30; ++i)
    {
        pageTable[i] = 0;
    }
}

int ALLOCATE()
{
    cout << "\nInside ALLOCATE\n";
    int temp;
    temp = rand() % 30;
    while (pageTable[temp] == 1)
    {
        temp = rand() % 30;
    }
    pageTable[temp] = 1;

    return temp;
}

void MOS()
{
    cout << "\nInside MOS\n";

    if (PI != 0)
    {
        //* IF PI is 1, 2 or 3 - Cases for TI and PI
        if (TI == 0)
        {
            if (PI == 1)
            {
                TERMINATE(4);
            }
            else if (PI == 2)
            {
                TERMINATE(5);
            }
            else if (PI == 3)
            {
                if ((IR[0] == 'G' && IR[1] == 'D') || (IR[0] == 'S' && IR[1] == 'R'))
                {
                    int realAddress = ((int)IR[2] - 48) * 10 + ((int)IR[3] - 48);
                    realAddress = PTR + realAddress / 10;

                    int temp = ALLOCATE();
                    cout << "\nValid Page Fault. Block No - " << temp << endl;

                    memory[realAddress][0] = '_';
                    memory[realAddress][1] = '_';
                    memory[realAddress][2] = temp / 10 + '0';
                    memory[realAddress][3] = temp % 10 + '0';

                    --IC;
                    PI = 0;
                    EXECUTE_USER_PROGRAM();
                }
                else
                {
                    //* Invalid Page Fault
                    TERMINATE(6);
                }
            }
        }
        else if (TI == 2)
        {
            if (PI == 1)
            {
                TERMINATE(7);
            }
            else if (PI == 2)
            {
                TERMINATE(8);
            }
            else if (PI == 3)
            {
                TERMINATE(3);
            }
        }
    }
    else
    {
        //* IF PI is 0 - Cases for TI and SI
        if (TI == 0)
        {
            if (SI == 1)
            {
                READ();
            }
            else if (SI == 2)
            {
                WRITE();
            }
            else if (SI == 3)
            {
                TERMINATE(0);
            }
        }
        else if (TI == 2)
        {
            if (SI == 1)
            {
                TERMINATE(3);
            }
            else if (SI == 2)
            {
                WRITE();
                TERMINATE(3);
            }
            else if (SI == 3)
            {
                TERMINATE(0);
            }
        }
    }
}

void TERMINATE(int EM)
{
    cout << "\nInside TERMINATE\n";

    foutput << "JOB ID : " << jobs[noOfJobs].JID << ", ";
    switch (EM)
    {
    case 0:
        foutput << "No ERROR";
        break;

    case 1:
        foutput << "OUT OF DATA";
        break;

    case 2:
        foutput << "LINE LIMIT EXCEEDED";
        break;

    case 3:
        foutput << "TIME LIMIT EXCEEDED";
        break;

    case 4:
        foutput << "OPCODE ERROR";
        break;

    case 5:
        foutput << "OPERAND ERROR";
        break;

    case 6:
        foutput << "INVALID PAGE FAULT";
        break;

    case 7:
        foutput << "TIME LIMIT EXCEEDED AND OPCODE ERROR";
        break;

    case 8:
        foutput << "TIME LIMIT EXCEEDED AND OPERAND ERROR";
        break;

    default:
        break;
    }
    foutput << "\nIC : " << IC << ", ";
    if (IR[0] == 'H')
    {
        foutput << "IR : " << IR[0];
    }
    else
    {
        foutput << "IR : " << string(IR.begin(), IR.end());
    }
    foutput << ", ";

    foutput << "TTC :" << jobs[noOfJobs].TTC << ", ";
    foutput << "LLC :" << jobs[noOfJobs].LLC << ", ";
    foutput << "TTL :" << jobs[noOfJobs].TTL << ", ";
    foutput << "TLL :" << jobs[noOfJobs].TLL << endl;
    foutput << endl;
    foutput << endl;
}

void READ()
{
    cout << "\nInside READ\n";
    SI = 0;
    IR[3] = '0';
    int val = ((int)IR[2] - 48) * 10 + ((int)IR[3] - 48);
    string buffer;
    char temp;
    getline(finput, buffer);

    cout << buffer << endl;
    //* Check for $END
    if (buffer[0] != '$')
    {
        int realAddress = ADDRESS_MAP(val);
        int j = 0;
        while (j < 40 && j < buffer.length())
        {
            cout << "M[" << realAddress << "]: ";
            for (int i = 0; i < 4 && j < buffer.length(); ++i)
            {
                memory[realAddress][i] = buffer[j++];
                cout << memory[realAddress][i] << " ";
            }
            cout << endl;
            ++realAddress;
        }
        EXECUTE_USER_PROGRAM();
    }
    else
    {
        TERMINATE(1);
    }
}

void WRITE()
{
    cout << "\nInside WRITE\n";
    SI = 0;
    IR[3] = '0';
    int val = ((int)IR[2] - 48) * 10 + ((int)IR[3] - 48);
    ++jobs[noOfJobs].LLC;
    if (jobs[noOfJobs].LLC > jobs[noOfJobs].TLL)
    {
        TERMINATE(2);
    }
    else
    {
        int j = 0, i = 0;
        int realAddress = ADDRESS_MAP(val);
        while (i < 10)
        {
            if (memory[realAddress][j] != '\n' && memory[realAddress][j] != '_' && memory[realAddress][j] != '$')
            {
                foutput << memory[realAddress][j];
            }
            ++j;
            if (j == 4)
            {
                ++realAddress;
                ++i;
                j = 0;
            }
        }
        foutput << endl;
        if (TI == 0)
        {
            EXECUTE_USER_PROGRAM();
        }
    }
}