#include "main.h"

string *split_string(string str, int &num)
{
    string *arr = new string[4];
    arr[1] = "";
    arr[2] = "";
    arr[3] = "";
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

table *find_max(restaurant *r)
{
    table *temp = r->recentTable->next;
    table *max = temp;
    while (temp != r->recentTable)
    {
        if (temp->age > max->age)
        {
            max = temp;
        }
        temp = temp->next;
    }
    if (temp->age > max->age)
    {
        max = temp;
    }
    return max;
}

void swap(table *a, table *b)
{
    string c = a->name;
    int d = a->age;
    a->age = b->age;
    a->name = b->name;
    b->age = d;
    b->name = c;
}

void selection_sort(restaurant *r, int num)
{
    table *temp;
    table *tmp = r->recentTable;
    table *current_max;

    // let max age be recentable
    table *max = find_max(r);
    swap(max, r->recentTable);

    for (int i = 1; i <= num; i++)
    {
        temp = r->recentTable;
        current_max = temp;
        while (temp != tmp)
        {
            if (temp->age > current_max->age)
            {
                current_max = temp;
            }
            if (temp->next == tmp)
            {
                if (current_max->age > temp->age)
                {
                    swap(current_max, temp);
                }
                tmp = temp;
                break;
            }
            temp = temp->next;
        }
    }
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
    int changeID = 0;
    string changeName = "";
    int changeAge = 0;

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

                            changeID = temp->ID;
                            changeName = temp->name;

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
                        changeID = temp->ID;
                        changeName = temp->name;

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
                print_queue1->recentTable->next = new table(0, arr[1], stoi(arr[2]), print_queue1->recentTable->next);
                print_queue2->recentTable = print_queue2->insert(print_queue2->recentTable, 0, arr[1], stoi(arr[2]));
                print_queue2->recentTable->name = arr[1];
            }
        }
        else if (arr[0] == "REGM")
        {
            if (merge)
                continue;
            table *tmp = r->recentTable->next;
            table *pos = tmp;
            int i = 1;
            int j = 0;
            table *currAvailSeat = new table(0, "", 0, NULL);

            while (i <= MAXSIZE)
            {
                if (j == stoi(arr[3]))
                {
                    if (tmp->ID > currAvailSeat->ID)
                    {
                        currAvailSeat = tmp;
                    }
                    tmp = tmp->next;
                    i++;
                    pos = tmp;
                    j = 0;
                }
                else
                {
                    if (pos->name == "")
                    {
                        pos = pos->next;
                        j++;
                    }
                    else
                    {
                        tmp = tmp->next;
                        i++;
                        pos = tmp;
                        j = 0;
                    }
                }
            }

            table *temp = currAvailSeat;
            table *delNode = currAvailSeat->next;
            if (currAvailSeat->ID)
            {
                currAvailSeat->name = arr[1];
                currAvailSeat->age = stoi(arr[2]);
                for (int i = 0; i < stoi(arr[3]); i++)
                {

                    if (currAvailSeat->ID == MAXSIZE)
                    {
                        r->recentTable = temp;
                    }
                    if (i != stoi(arr[3]) - 1)
                        currAvailSeat = currAvailSeat->next;
                }
                temp->next = currAvailSeat->next;
                currAvailSeat->next = nullptr;
                delete delNode;

                merge_num = stoi(arr[3]);
                merge_id = temp->ID;

                changeID = temp->ID;
                changeName = temp->name;

                print_queue1->recentTable->next = new table(temp->ID, temp->name, temp->age, print_queue1->recentTable->next);

                merge = true;
            }
        }
        else if (arr[0] == "CLE")
        {
            table *temp = r->recentTable->next;
            int index = stoi(arr[1]);

            // if (merge)
            // {
            if (merge && (index >= merge_id && index < merge_id + merge_num))
            {
                merge = false;
                cout << "is merge: " << merge << endl;
                table *temp1 = r->recentTable;
                int j = 1;
                while (temp1->ID != merge_id)
                {
                    temp1 = temp1->next;
                }

                changeID = merge_id;
                changeName = temp1->name;

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
                continue;
            }
            // }

            int i = 1;
            while (i != index)
            {
                temp = temp->next;
                i++;
            }

            if (temp->name != "")
            {
                changeID = temp->ID;
                changeName = temp->name;
                changeAge = temp->age;

                if (queue->recentTable)
                {
                    table *delNode = queue->recentTable->next;
                    queue->recentTable->next = delNode->next;
                    delNode->next = nullptr;

                    temp->name = delNode->name;
                    temp->age = delNode->age;

                    cout << temp->name << "-" << temp->age << endl;

                    if (print_queue1->recentTable)
                    {
                        if (print_queue1->recentTable->name == temp->name && print_queue1->recentTable->age == temp->age)
                        {
                            cout << "asdasd" << endl;
                            table *tmp1 = print_queue1->recentTable->next;
                            cout << tmp1->name << endl;
                            while (tmp1->next != print_queue1->recentTable)
                            {
                                tmp1 = tmp1->next;
                            }
                            table *del = print_queue1->recentTable;
                            tmp1->next = del->next;
                            del->next = nullptr;
                            delete del;
                            print_queue1->recentTable = tmp1;
                        }
                        else
                        {
                            table *tmp1 = print_queue1->recentTable->next;
                            cout << "asdasd" << endl;
                            if (tmp1->name == delNode->name && tmp1->age == delNode->age)
                            {
                                cout << "cuong" << endl;
                                print_queue1->recentTable->next = tmp1->next;
                                tmp1->next = nullptr;
                                delete tmp1;
                            }
                            else
                            {
                                cout << "cuong" << endl;
                                while (tmp1 != print_queue1->recentTable)
                                {
                                    if (tmp1->next->name == delNode->name && tmp1->next->age == delNode->age)
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
                        cout << "asdasd" << endl;
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

                    temp->name = "";
                    temp->age = 0;
                }
            }
        }
        else if (arr[0] == "PS")
        {
            if (print_queue1->recentTable == nullptr)
            {
                cout << "Empty\n";
                continue;
            }
            int n = -1;
            int i = 0;
            table *temp;
            if (print_queue1->recentTable->next)
                temp = print_queue1->recentTable->next;
            else
            {
                cout << print_queue1->recentTable->name << "\n";
                continue;
            }
            if (arr[1] != "")
                n = stoi(arr[1]);
            while (true)
            {
                if (i == n)
                    break;
                cout << temp->name << "\n";
                i++;
                if (temp->next == print_queue1->recentTable->next)
                    break;
                temp = temp->next;
            }
        }
        else if (arr[0] == "PT")
        {
            if (changeID != 0)
            {
                table *temp = r->recentTable;
                while (temp->ID != changeID)
                {
                    temp = temp->next;
                }
                cout << changeID << "-" << changeName << "\n";
                temp = temp->next;
                while (temp->ID != changeID)
                {
                    if (temp->name == "")
                    {
                        temp = temp->next;
                        continue;
                    }
                    cout << temp->ID << "-" << temp->name << "\n";
                    temp = temp->next;
                }
            }
        }
        else if (arr[0] == "PQ")
        {
            if (print_queue2->recentTable == nullptr)
            {
                cout << "Empty\n";
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
                cout << temp->name << "\n";
                i++;
                if (temp->next == print_queue2->recentTable->next)
                    break;
                temp = temp->next;
            }
            cout << temp->name << "\n";
        }
        else if (arr[0] == "SQ")
        {
            if (!queue->recentTable)
            {
                cout << "Empty\n";
            }
            else
            {
                selection_sort(queue, stoi(arr[1]));
                table *temp = queue->recentTable->next;
                while (temp != queue->recentTable)
                {
                    cout << temp->name << endl;
                    temp = temp->next;
                }
                cout << temp->name << endl;
            }
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
        else if (arr[0] == "PRINTPQ")
        {
            if (print_queue1->recentTable->next)
            {
                table *temp = print_queue1->recentTable->next;
                while (temp != print_queue1->recentTable)
                {
                    cout << "[" << temp->ID << " " << temp->name << " " << temp->age << "]->";
                    temp = temp->next;
                }
                cout << "[" << temp->ID << " " << temp->name << " " << temp->age << "]->[]" << endl;
            }
            else
                cout << "[" << print_queue1->recentTable->ID << " " << print_queue1->recentTable->name << " " << print_queue1->recentTable->age << "]->[]" << endl;
        }
        delete[] arr;
    }
    delete queue;
    delete print_queue1;
    delete print_queue2;
    file.close();
}
