#include "main.h"

string *split_string(string str, int &num)
{
    string *arr = new string[4];
    int count = 0;
    for (int i = 0; i < str.length(); i++)
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

table *check_merge(table *last, int num)
{
    if (num == 0)
        return last;
}

void simulate(string filename, restaurant *r)
{
    string row;
    string *arr;
    int count;
    bool merge = false;
    int merge_num = 0;

    // Queue
    restaurant *queue = new restaurant();

    // Print PS queue
    restaurant *print_queue1 = new restaurant();

    // Print PQ queue
    restaurant *print_queue2 = new restaurant();

    ifstream file("test.txt");

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
                int i = 1;
                int index = stoi(arr[1]);
                while (true)
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
                            cout << temp->ID << " " << temp->name << " " << temp->age << endl;
                            print_queue1->recentTable = print_queue1->insert(print_queue1->recentTable, temp->ID, temp->name, temp->age);
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
                int i = 1;
                while (i <= MAXSIZE)
                {
                    if (temp->name == "")
                    {
                        temp->age = stoi(arr[2]);
                        temp->name = arr[1];
                        isBooked2 = true;
                        cout << temp->ID << " " << temp->name << " " << temp->age << endl;
                        print_queue1->recentTable = print_queue1->insert(print_queue1->recentTable, temp->ID, temp->name, temp->age);
                        break;
                    }
                    temp = temp->next;
                    i++;
                }
            }
            if (isBooked1 == false && count == 3)
            {
                queue->recentTable = queue->insert(queue->recentTable, stoi(arr[1]), arr[2], stoi(arr[3]));
                print_queue1->recentTable = print_queue1->insert(print_queue1->recentTable, temp->ID, arr[2], stoi(arr[3]));
                print_queue2->recentTable = print_queue2->insert(print_queue2->recentTable, stoi(arr[1]), arr[2], stoi(arr[3]));
            }
            if (isBooked2 == false && count == 2)
            {
                queue->recentTable = queue->insert(queue->recentTable, 0, arr[2], stoi(arr[3]));
                print_queue1->recentTable = print_queue1->insert(print_queue1->recentTable, 0, arr[1], stoi(arr[2]));
                print_queue2->recentTable = print_queue2->insert(print_queue2->recentTable, 0, arr[1], stoi(arr[2]));
            }
        }
        else if (arr[0] == "REGM")
        {
            if (merge)
                continue;
            if (r->recentTable->name == "")
            {
                merge_num++;
            }
            table *temp = r->recentTable->next;
            table *tmp = temp;
            while (temp != r->recentTable)
            {
                if (merge_num < stoi(arr[1]) && temp->name != "")
                {
                    merge_num = 0;
                    tmp = temp;
                }

                merge_num++;

                if (merge_num == stoi(arr[1]))
                    break;

                temp = temp->next;
            }
            if (merge_num == stoi(arr[1]))
            {
            }
        }
        else if (arr[0] == "CLE")
        {
            table *temp = r->recentTable->next;
            int index = stoi(arr[1]);
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

                    table *tmp1 = print_queue1->recentTable->next;
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
                    table *tmp2 = print_queue2->recentTable->next;
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

                    delete delNode;
                }
                else
                {
                    temp->name = "";
                    temp->age = 0;
                }
            }

            if (merge == true)
            {
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
            table *temp = print_queue1->recentTable->next;
            while (temp != print_queue1->recentTable)
            {
                if (i == stoi(arr[1]))
                    break;
                if (temp->name == "")
                    break;
                cout << temp->name << endl;
                i++;
                temp = temp->next;
            }
        }
        else if (arr[0] == "PQ")
        {
            if (print_queue2 == nullptr)
            {
                cout << "Empty" << endl;
                continue;
            }
            int i = 0;
            table *temp = print_queue2->recentTable->next;
            while (temp != print_queue2->recentTable)
            {
                if (i == stoi(arr[1]) || temp->name == "")
                {
                    break;
                }
                cout << temp->name << endl;
                i++;
                temp = temp->next;
            }
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
    }
    file.close();
}
