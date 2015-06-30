#include "Storage.h"

Storage::Storage()
{
	Row = 0;
	Column = 0; 
	Data = 0;
	Part_Id =0;
	det_tar = false; 
}
void Storage::set_row(int row_number)
{
	Row = row_number;
}					
void Storage::set_column(int col_number)
{
	Column = col_number;
}
void Storage::set_data(int data)			
{
	Data = data; 
}
void Storage::set_pid(int part_id)
{
	Part_Id = part_id;
}
void Storage::set_tar(bool tar)
{
	det_tar = tar; 
}
int Storage::get_row()const
{
	return Row; 
}
int Storage::get_column()const
{
	return Column;
}
int Storage::get_data()const
{
	return Data; 
}
int Storage::get_pid() const
{
	return Part_Id; 
}
bool Storage::get_tar() const 
{
	return det_tar;
}
	
Storage & Storage:: operator=(const Storage &rhs)
{
	if(this == &rhs)
	{
		return *this;
	}	
	Row = rhs.get_row();
	Column = rhs.get_column();
	Data = rhs.get_data();
	Part_Id = rhs.get_pid();
	return *this; 
}
Storage::~Storage()
{
}




