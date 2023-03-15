#include "main.h"

string *split_string(string str, int &num)
{
    string *arr = new string[4];
    int count = 0;
    int n = str.length();
    for (int i = 0; i < n; i++)
    {
        if (str[i] == ' ')
        {
            count++;
            continue;
        }
        arr[count] += str[i];
    }
    num = count;
    return arr;
}

void simulate(string filename, restaurant *r)
{
    string row;
    string *arr;
    int count;
    bool merge = false;
    int merge_num = 0;
    int merge_id = 0;
    int size = 0;

    // Queue
    restaurant *queue = new restaurant();

    // Print PS queue
    restaurant *print_queue1 = new restaurant();

    // Print PQ queue
    restaurant *print_queue2 = new restaurant();

    ifstream file(filename);

    while (getline(file, row))
    {
        arr = split_string(row, count);

        if (arr[0] == "REG")
        {
            table *temp = r->recentTable->next;
            bool isBooked1 = false;
            bool isBooked2 = false;
            // Booking with ID
            if (count == 3)
            {
                if (stoi(arr[3]) <= 16)
                    continue;
                int i = 1;
                int index = stoi(arr[1]);
                while (i <= MAXSIZE)
                {
                    if (i == index)
                    {
                        if (temp->name != "")
                        {
                            if (index == MAXSIZE)
                            {
                                temp = r->recentTable;
                                index = 1;
                                i = 1;
                            }
                            else
                                index++;
                        }
                        else
                        {
                            isBooked1 = true;
                            temp->age = stoi(arr[3]);
                            temp->name = arr[2];
                            if (!print_queue1->recentTable)
                            {
                                print_queue1->recentTable = print_queue1->finsert(print_queue1->recentTable, temp->ID, temp->name, temp->age);
                                print_queue1->recentTable->name = temp->name;
                            }
                            else
                            {
                                print_queue1->recentTable->next = new table(temp->ID, temp->name, temp->age, print_queue1->recentTable->next);
                            }
                            break;
                        }
                    }
                    temp = temp->next;
                    i++;
                }
            }
            // Booking without ID
            else if (count == 2)
            {
                if (stoi(arr[2]) <= 16)
                    continue;
                int i = 1;
                while (i <= MAXSIZE)
                {
                    if (temp->name == "")
                    {
                        temp->age = stoi(arr[2]);
                        temp->name = arr[1];
                        // cout << r->recentTable->next->name << r->recentTable->next->age << endl;
                        isBooked2 = true;
                        if (!print_queue1->recentTable)
                        {
                            print_queue1->recentTable = print_queue1->finsert(print_queue1->recentTable, 0, temp->name, temp->age);
                            print_queue1->recentTable->name = temp->name;
                        }
                        else
                        {
                            print_queue1->recentTable->next = new table(0, temp->name, temp->age, print_queue1->recentTable->next);
                        }
                        break;
                    }
                    temp = temp->next;
                    i++;
                }
                size++;
            }
            if (isBooked1 == false && count == 3)
            {
                queue->recentTable = queue->insert(queue->recentTable, stoi(arr[1]), arr[2], stoi(arr[3]));
                print_queue1->recentTable->next = new table(stoi(arr[1]), arr[2], stoi(arr[3]), print_queue1->recentTable->next);
                print_queue2->recentTable = print_queue2->insert(print_queue2->recentTable, stoi(arr[1]), arr[2], stoi(arr[3]));
                print_queue2->recentTable->name = arr[2];
            }
            if (isBooked2 == false && count == 2)
            {
                queue->recentTable = queue->insert(queue->recentTable, 0, arr[1], stoi(arr[2]));
                print_queue1->recentTable->next = new table(0, arr[2], stoi(arr[3]), print_queue1->recentTable->next);
                print_queue2->recentTable = print_queue2->insert(print_queue2->recentTable, 0, arr[1], stoi(arr[2]));
                print_queue2->recentTable->name = arr[1];
            }
        }
        else if (arr[0] == "REGM")
        {
        }
        else if (arr[0] == "CLE")
        {
            table *temp = r->recentTable->next;
            int index = stoi(arr[1]);

            if (merge)
            {
                table *temp1 = r->recentTable;
                if (index >= merge_id && index < merge_id + merge_num)
                {
                    int j = 1;
                    while (temp1->ID != merge_id)
                    {
                        temp1 = temp1->next;
                    }
                    temp1->name = "";
                    temp1->age = 0;
                    for (int i = 1; i < merge_num; i++)
                    {
                        table *newNode;
                        if (temp1->ID < 15)
                            newNode = new table(temp1->ID + 1, "", 0, nullptr);
                        else
                        {
                            newNode = new table(j, "", 0, nullptr);
                            j++;
                        }
                        newNode->next = temp1->next;
                        temp1->next = newNode;
                        temp1 = newNode;
                        if (temp1->ID == 15)
                        {
                            r->recentTable = temp1;
                        }
                    }
                }
                continue;
            }

            int i = 1;
            while (i != index)
            {
                temp = temp->next;
                i++;
            }

            if (temp->name != "" && merge == false)
            {
                if (queue->recentTable)
                {
                    table *delNode = queue->recentTable->next;
                    queue->recentTable->next = delNode->next;
                    delNode->next = nullptr;

                    temp->name = delNode->name;
                    temp->age = delNode->age;

                    if (print_queue1->recentTable)
                    {
                        table *tmp1 = print_queue1->recentTable->next;
                        if (tmp1->name == delNode->name)
                        {
                            print_queue1->recentTable->next = tmp1->next;
                            tmp1->next = nullptr;
                            delete tmp1;
                        }
                        else
                        {
                            while (tmp1 != print_queue1->recentTable)
                            {
                                if (tmp1->next->name == delNode->name)
                                {
                                    table *node = tmp1->next;
                                    tmp1->next = node->next;
                                    node->next = nullptr;
                                    delete node;
                                    break;
                                }
                                tmp1 = tmp1->next;
                            }
                        }
                    }

                    if (print_queue2->recentTable)
                    {
                        table *tmp2 = print_queue2->recentTable->next;
                        if (tmp2->name == delNode->name)
                        {
                            print_queue2->recentTable->next = tmp2->next;
                            tmp2->next = nullptr;
                            delete tmp2;
                        }
                        else
                        {
                            while (tmp2 != print_queue2->recentTable)
                            {
                                if (tmp2->next->name == delNode->name)
                                {
                                    table *node = tmp2->next;
                                    tmp2->next = node->next;
                                    node->next = nullptr;
                                    delete node;
                                    break;
                                }
                                tmp2 = tmp2->next;
                            }
                        }
                    }

                    delete delNode;
                }
                else
                {
                    if (print_queue1->recentTable)
                    {
                        table *tmp1 = print_queue1->recentTable->next;
                        if (tmp1->name == temp->name)
                        {
                            print_queue1->recentTable->next = tmp1->next;
                            tmp1->next = nullptr;
                            delete tmp1;
                        }
                        else
                        {
                            while (tmp1 != print_queue1->recentTable)
                            {
                                if (tmp1->next->name == temp->name)
                                {
                                    table *node = tmp1->next;
                                    tmp1->next = node->next;
                                    node->next = nullptr;
                                    delete node;
                                    break;
                                }
                                tmp1 = tmp1->next;
                            }
                        }
                    }

                    if (print_queue2->recentTable)
                    {
                        table *tmp2 = print_queue2->recentTable->next;
                        if (tmp2->name == temp->name)
                        {
                            print_queue2->recentTable->next = tmp2->next;
                            tmp2->next = nullptr;
                            delete tmp2;
                        }
                        else
                        {
                            while (tmp2 != print_queue2->recentTable)
                            {
                                if (tmp2->next->name == temp->name)
                                {
                                    table *node = tmp2->next;
                                    tmp2->next = node->next;
                                    node->next = nullptr;
                                    delete node;
                                    break;
                                }
                                tmp2 = tmp2->next;
                            }
                        }
                    }

                    temp->name = "";
                    temp->age = 0;
                }
            }
        }
        else if (arr[0] == "PS")
        {
            if (print_queue1->recentTable == nullptr)
            {
                cout << "Empty" << endl;
                continue;
            }
            int i = 0;
            table *temp;
            if (print_queue1->recentTable->next)
                temp = print_queue1->recentTable->next;
            else
            {
                cout << print_queue1->recentTable->name << endl;
                continue;
            }
            // cout << temp->name << endl;
            while (true)
            {
                if (i == stoi(arr[1]))
                    break;
                cout << temp->name << endl;
                i++;
                temp = temp->next;
            }
        }
        else if (arr[0] == "PT")
        {
        }
        else if (arr[0] == "PQ")
        {
            if (print_queue2->recentTable == nullptr)
            {
                cout << "Empty" << endl;
                continue;
            }
            int i = 0;
            table *temp;
            if (print_queue2->recentTable->next)
                temp = print_queue2->recentTable->next;
            else
                temp = print_queue2->recentTable;
            while (temp != print_queue2->recentTable)
            {
                if (i == stoi(arr[1]))
                {
                    break;
                }
                cout << temp->name << endl;
                i++;
                temp = temp->next;
            }
            cout << temp->name << endl;
        }
        else if (arr[0] == "SQ")
        {
        }
        else if (arr[0] == "PRINT")
        {
            table *temp = r->recentTable->next;
            while (temp != r->recentTable)
            {
                cout << "[" << temp->ID << " " << temp->name << " " << temp->age << "]->";
                temp = temp->next;
            }
            cout << "[" << temp->ID << " " << temp->name << " " << temp->age << "]->[]" << endl;
        }
    }
    file.close();
}
