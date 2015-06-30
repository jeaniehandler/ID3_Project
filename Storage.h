#ifndef STORAGE_H
#define STORAGE_H

#include <iostream>

using namespace std;

class Storage
{
	private:
		int Row;
		int Column;
		int Data;
		int Part_Id;  
		bool det_tar;						
	public: 
		Storage();
		Storage & operator=(const Storage &rhs);
		void set_row(int row_number);
		void set_column(int col_number);
		void set_data(int data);
		void set_pid(int part_id);
		void set_tar(bool tar);
		int get_row()const;
		int get_column()const;
		int get_data()const;
		int get_pid()const; 	
		bool get_tar()const;
		~Storage();

};
#endif 
