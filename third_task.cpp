#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>

#include "errors.h"

// структуру ListNode модифицировать нельзя
struct ListNode {
	ListNode* prev;
	ListNode* next;
	ListNode* rand; // указатель на произвольный элемент данного списка, либо NULL
	std::string data;
};

class List {
public:
	void Serialize(FILE* file); // сохранение в файл (файл открыт с помощью fopen(path, "wb"))
	void Deserialize(FILE* file); // загрузка из файла (файл открыт с помощью fopen(path, "rb"))
private:
	ListNode* head = nullptr;
	ListNode* tail = nullptr;
	int count;
	const char* separator = " ";
	const char not_separator = '|'; // для дефолтного значения, т.к. буду считывать по символьно до разделителя
	void Clear();
};

void List::Serialize(FILE* file)
{
	ListNode* node = head;
	std::unordered_map<ListNode*, int> map;
	int i = 1; // т.к. atoi возвращает 0 если не получилось преобразовать, то индексирую с 1(только для сериализации и десериализации)
	while (node)
	{
		map[node] = i;
		node = node->next;
		i++;
	}
	map[nullptr] = -1;

	node = head;

	fputs((std::to_string(count).c_str()), file);
	fputs(separator, file);
	
	while (node)
	{
		fputs(std::to_string(map[node]).c_str(), file);
		fputs(separator, file);

		fputs(std::to_string(map[node->rand]).c_str(), file);
		fputs(separator, file);

		fputs(std::to_string(node->data.size()).c_str(), file);
		fputs(separator, file);

		fputs(node->data.c_str(), file);
		
		node = node->next;
	}
}

void List::Clear()
{
	ListNode* node = head;
	while (node)
	{
		ListNode* temp = node->next;
		delete node;
		node = temp;
	}

	head = tail = nullptr;
	count = 0;
}

void List::Deserialize(FILE* file)
{
	Clear();
	std::unordered_map<int, int> map_for_rand; // для каждого индекса на какой(по индексу) он указывает в rand
	std::vector<ListNode*> list_node_index_map; // для каждого индекса храню его адрес 
	std::string size = "";
	char buffer[2] = { not_separator };

	while (!feof(file) && buffer[0] != separator[0])
	{
		fgets(buffer, 2, file);
		size += buffer[0];
	}
	count = atoi(size.c_str());
	if (count == 0 || feof(file))
	{
		fclose(file);
		return;
	}
	ListNode* cur_node;
	int cur_index = 1; // т.к. atoi возвращает 0 если не получилось преобразовать, то индексирую с 1(только для сериализации и десериализации)
	while (!feof(file) && cur_index <= count)
	{
		buffer[0] = not_separator;
		std::string cur_index_in_file = "";
		while (!feof(file) && buffer[0] != separator[0])
		{
			fgets(buffer, 2, file);
			cur_index_in_file += buffer[0];
		}
		if (cur_index != atoi(cur_index_in_file.c_str()) || feof(file))
		{
			fclose(file);
			throw NodeIndexException(cur_index);
		}

		buffer[0] = not_separator;
		std::string rand_index_str = "";
		while (!feof(file) && buffer[0] != separator[0])
		{
			fgets(buffer, 2, file);
			rand_index_str += buffer[0];
		}
		
		int rand_index = atoi(rand_index_str.c_str());
		if (rand_index == 0 || feof(file))
		{
			fclose(file);
			throw NodeRandException(cur_index);
		}

		buffer[0] = not_separator;
		std::string data_len_str = "";
		while (!feof(file) && buffer[0] != separator[0])
		{
			fgets(buffer, 2, file);
			data_len_str += buffer[0];
		}

		int len = atoi(data_len_str.c_str());
		if (feof(file))
		{
			fclose(file);
			throw NodeLenException(cur_index);
		}

		buffer[0] = not_separator;
		std::string data = "";
		int data_ch_counter = 0;
		while (!feof(file) && data_ch_counter < len)
		{
			fgets(buffer, 2, file);
			data += buffer[0];
			data_ch_counter++;
		}

		if (data.size() != len)
		{
			fclose(file);
			throw NodeDataException(cur_index);
		}

		map_for_rand[cur_index] = rand_index;
		cur_node = new ListNode{ nullptr, nullptr, nullptr, data };
		if (head == nullptr)
		{
			head = cur_node;
		}
		list_node_index_map.push_back(cur_node);
		cur_node = cur_node->next;
		cur_index++;
	}

	if (list_node_index_map.size() != count)
	{
		fclose(file);
		throw CountException();
	}
	fgets(buffer, 2, file); //считываю \0
	if (!feof(file))
	{
		throw EofException();
	}

	for (size_t i = 0; i < list_node_index_map.size(); i++)
	{
		if (i != 0)
		{
			list_node_index_map[i]->prev = list_node_index_map[i - 1];
		}
		if ((i + 1) < list_node_index_map.size())
		{
			list_node_index_map[i]->next = list_node_index_map[i + 1];
		}
		if (map_for_rand[i + 1] != -1)  //т.к. индексировал с 1
		{
			list_node_index_map[i]->rand = list_node_index_map[map_for_rand[i + 1] - 1]; //т.к. индексировал с 1, а в векторе индексация с 0
		}
	}
}

