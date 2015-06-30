#include <fstream>
#include <sstream>
#include <string>
#include "Storage.h" 
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define FILES 3
#define START 0
#define MAX_INPUT 250

int main()
{			
	char input[MAX_INPUT];	
	string split[FILES]; 	
	////////////////////////////////////////////////////////User Input			
	cout << "Enter file names : Dataset Input-partition Output-partition\n"; 
	cin.getline(input,sizeof(input)); 
	
	string str_i = string(input); 

	for(int i = START; i < FILES ; i++)
	{
		int space = str_i.find(" ");
		if (space != -1)
		{					
			string file = str_i.substr(START,space); 
			int size = file.length() + 1; 
			split[i] = file;
			str_i = str_i.erase(START,size); 
		}
		else
		{
			split[i] = str_i; 
		}
	}
	
	string data = split[0];
	string in_f = split[1];
 	string out_f = split[2]; 
	/////////////////////////////////////////////////////////Reading the first File
	ifstream inputFile(data.c_str());
	string read;
	int c = START;  
	int a = START; 
	int value; 
	int features;
	int examples;
	istringstream stream; 
	if(!inputFile.is_open())
	{	
		cout << "Error Reading Input File" << endl;
		return 0; 
	}	
	//read first line of dataset to get examples and features
	getline(inputFile,read); 
	stream.str(read);
	stream >> examples >> features; 	
	//multiply to get total amount of data
	int total = examples * features; 
	Storage id[total];  
	//Loop through each row
	while(c < examples)
	{	
		stream.clear();
		getline(inputFile,read);
		stream.str(read);
		//Loop through each column and set attributes accordingly  
		for(int f = START; f < features; f++)
		{	
			stream >> value;
			id[a].set_row(c);
			id[a].set_column(f);
			id[a].set_data(value); 		
			//set the last column to true to specify target feature
			if(f == features-1)
			{
				id[a].set_tar(true);
			}
			else 
			{
				id[a].set_tar(false); 
			}
			a++; 
		}
		c++; 
	}
	inputFile.close();
	stream.clear(); 


	////////////////////////////////////////Reading the second file 
	inputFile.open(in_f.c_str());
	if(!inputFile.is_open())
	{	
		cout << "Error Reading Partition File" << endl;
		return 0; 
	}
	int pid; 
	int pieces;
	int pidcount = START;  
	while(getline(inputFile,read))
	{
		stream.str(read);
		stream >> pid;
		pidcount++;
		while (stream >> pieces)
		{
			for(int k = START; k < total;k++)
			{
				if(id[k].get_row() == pieces-1)
				{			
					id[k].set_pid(pid);
				}
			}
		}
		stream.clear(); 
	}
	inputFile.close(); 	
	stream.clear();

	inputFile.open(in_f.c_str());
	if(!inputFile.is_open())
	{	
		cout << "Error Reading Partition File" << endl;
	}
	int array[pidcount];
	string str_array[pidcount];  
	int u = START;
	while(getline(inputFile,read))
	{
		stream.str(read);
		stream >> pid;
		array[u] = pid;
		str_array[u] = read;  
		u++; 
		stream.clear();
	}
	inputFile.close(); 
	stream.clear(); 
	///////////////////////////////////////////////start calculations
	
	int partID;
	
	int n =0;
	int m = 0; 
	
	int row[examples]; 
	bool par[examples];
	int target[examples];
	
	int part_s = 0;
	int tar_zero = 0;
	int tar_one = 0; 
	int z_col=0;
	int o_col=0;
	int com_zero_s = 0;
	int com_one_s = 0; 
	int zero_s = 0; 
	int one_s = 0; 
	
	float prob_tar_zero;
	float prob_tar_one;
	float o_ent;
	float z_ent; 
	
	float prob_c_z; 	
	float prob_c_o; 	

	float ent_z; 
	float ent_o; 
	float entropy; 
	float ent_p_o;
	float ent_p_z;
	float final_ent; 
	float gain; 
	float final_gain; 
	float highest = 0.0; 
	int partition=0; 
	int feat=0; 
	int write[examples]; 
	for(int t = START; t < pidcount; t++)
	{			
		partID = array[t];
		//cout << "Partition ID " << partID << endl; 
		for(int s = START; s < features - 1; s++)
		{					
			//cout << "Finding Entropy for Column : " << s + 1 << endl; 
			for(int w = START; w < total; w++)
			{
			
				/*	cout << " R : " << id[w].get_row() 
					     << " C : " << id[w].get_column()  
					     << " D : " << id[w].get_data() 
					     << " P : " << id[w].get_pid() 
					     << " T : " << id[w].get_tar()<<  endl;
				*/							
				
				//if Storage object is Target attribute, set target array to data value
				if(id[w].get_tar())
				{
					target[n] = id[w].get_data();
					n++;	
				}
				//if Strorage object is column specified by loop set column array to data value	
				if(id[w].get_column() == s)
				{
					row[m] = id[w].get_data(); 
					m++; 
				}
				//Test if Storage pid is greater or less than partID if so set to false							
				if(id[w].get_pid() > partID || id[w].get_pid() < partID)
				{
					par[id[w].get_row()]= false;
				}
				else 
				{
					par[id[w].get_row()]= true; 
				}
			}	
			//loop through newly populated arrays target,column,and row
			for (int r = START; r < examples; r++)
			{
				//		cout 	<< "ROW : " << row[r] 
				//			<< "PART : " << par[r] 
				//			<< "TAR : " << target[r] << endl;  
				
				//while looping through rows if partition specified
				//Set up counts to specify amounts of Zero's and One's 
				//for Column and Target Column
				if(par[r])
				{
					part_s++;//count the parts in the partition 
					
					if(target[r] == 0)
					{
						tar_zero++; //Zero's in Target Column
					}
					else
					{			
						tar_one++;//Ones's in Target Column
					}	
					if(row[r] == 0)
					{
						z_col++;//Zero's in Column being calculated
					}
					else
					{
						o_col++;//One's in Column being calculated
					}
					
					if(row[r] == 0 && target[r] == 0)
					{
						com_zero_s++; //Zero's in both Column and Target
					}
	
					if(row[r] == 1 && target[r] == 1)
					{		
						com_one_s++;//One's in both Column and Target
					}
				
					if(row[r] == 0 && target[r] ==1)
					{
						zero_s++;//Column has Zero, Target has One
					}						
					if(row[r] == 1 && target[r] == 0)
					{
						one_s++;//Column has a One, Target has Zero
					}
					
				}
	
			}
							
				prob_tar_zero = (float)tar_zero/(float)part_s;//prob target zero = targets_zeros/total_parts
				prob_tar_one =  (float)tar_one/(float)part_s;//prob target one = targets_ones/total_parts
				if(prob_tar_one > 0)
				{							
			 										
					o_ent =(float)(prob_tar_one * log2(1/prob_tar_one)); //entropy target (one) 
				}
				else 
				{
					o_ent = 0; 
				}	
				if(prob_tar_zero > 0)
				{								
					z_ent = (float)(prob_tar_zero * log2(1/prob_tar_zero));//entropy target(zero)
	 				
				}	
				else
				{
					z_ent = 0; 
				}
				entropy = o_ent + z_ent; //total target entropy = one_entropy + zero entropy
	
				//cout << " PROB TARGET Zero: " << prob_tar_zero << endl; 
				//cout << " PROB TARGET One: " << prob_tar_one << endl; 
				
				//cout << " TARGET ENTROPY : " << entropy << endl;  
				
				//Column given zeros
				if(z_col > 0)
				{
					prob_c_z = (float)com_zero_s /(float)z_col;//prob_combined_zeros = combined_zeros/zeros_in_column 
				}
				else
				{
					prob_c_z = 0; 
				}
				//Column given ones
				if(o_col > 0)
				{
					prob_c_o = (float)com_one_s/(float)o_col; 
				}
				else
				{
					prob_c_o = 0; 
				}
				//Column given not zeros
				float prob_z = (float) 1 - prob_c_z;
				//Column given not ones
				float prob_o = (float) 1 - prob_c_o; 	
			
				/*cout << "Prob_c_z" << prob_c_z << endl;    					
				cout << "Prob_c_o" << prob_c_o << endl;    					
				cout << "Prob_z" << prob_z << endl;    					
				cout << "Prob_o" << prob_o << endl;    					
				*/

				if(prob_c_z > 0)
				{
					ent_z =(float) (prob_c_z* log2(1/prob_c_z)); 
				}
				else
				{
					ent_z = 0;
				}
				if(prob_z > 0)
				{	
					ent_p_z= (float)(prob_z * log2(1/prob_z));
				}
				else 
				{
					ent_p_z = 0; 
				}
				float ent_f_z  = ent_z + ent_p_z;
				//cout << " ENTROPY GIVEN ZERO : " << ent_f_z << endl;
				if(prob_c_o > 0)
				{							
					ent_o =(float) (prob_c_o* log2(1/prob_c_o));
				}
				else
				{
					ent_o = 0; 
				}		
				if(prob_o > 0) 
				{	
					ent_p_o =  (float)(prob_o * log2(1/prob_o)); 
				}
				else 
				{
					ent_p_o = 0; 
				}
				float ent_f_o = ent_o + ent_p_o; 
				//cout << " ENTROPY GIVEN ONE : "  << ent_f_o << endl; 								
										
				float prob_zero = (float)z_col/(float)part_s; 
				float prob_one = (float)o_col/(float)part_s; 
				//float final_ent = (float)(prob_zero* ent_f_z) + (float)(prob_one * ent_f_o);
				final_ent = (float)(ent_f_z) + (float)(ent_f_o);
				//cout << " PROB TARGET ZERO * ENTROPY GIVEN ZERO + PROB TARGET ONE * ENTROPY GIVEN ONE : " << final_ent << endl; 
				gain= entropy - final_ent; 
				if (gain < 0)
				{
					final_ent = (float)(prob_zero* ent_f_z) + (float)(prob_one * ent_f_o);
					gain = entropy - final_ent; 
				}	
				/*cout << "z_prob" << z_prob << endl;    					
				cout << "o_prob" << o_prob << endl;    					
				cout << "ent_z" << ent_f_z << endl;    					
				cout << "ent_o" << ent_f_o << endl;    					
				cout << "final" << final_ent << endl;    					
			*/						
				//cout << " FINAL ENTROPY : " << final_ent << endl;
				final_gain = gain*((float)part_s/(float)examples);
				//cout << " GAIN : " << final_gain << endl; 
				if (highest  < final_gain)
				{
					highest = final_gain;
					partition = partID; 
					feat = s;  
				}
				
			/*
				cout << "Rows in Partition : "	<< part_s << endl;
				cout << "Zeros in Partition columnn : "	<< zero_s << endl;
				cout << "Ones in Partition columnn : "	<< one_s << endl;
				cout << "Zeros in Partition column and Target : "	<< com_zero_s << endl;
				cout << "Ones in Partition column and Target : "	<< com_one_s << endl;
			
			*/	part_s = 0;
				zero_s = 0;
				one_s = 0;
				com_zero_s = 0;
				com_one_s = 0; 	
				tar_zero = 0;
				tar_one = 0; 
				entropy = 0; 
				z_col=0; 
				o_col =0;
			
				m=0;
				n=0; 
		}
			
	}
	
		
						 	
		cout << "HIGHEST Gain : " <<highest << endl; 
		if(partition > 0)
		{
			cout << "Split Partition Id : " << partition << endl;
		}
		else
		{
			cout << "No Partition to Split " << endl; 
		}
	////////writing output file  
				
	srand(time(0));
	ofstream OutputFile;
	int row_num = 0; 
	OutputFile.open(out_f.c_str());
	string partition1; 
	string partition2; 
	ostringstream converting; 
	int flag_w = 0;  
	for (int v=0 ;v < pidcount;v++)
	{
		if(array[v] != partition)
		{
			OutputFile << str_array[v] << endl;
		}
		else if(array[v] == partition)
		{
			flag_w++; 
			cout << "Partition " << partition << " was split into ";
			string parse = str_array[v]; 
			stream.str(parse);
			stream >> pid; 
			cout << pid << " and "; 
			OutputFile << pid;
			OutputFile << " "; 
			while(stream >> pieces)
			{	
				for(int h=0;h<total;h++)
				{
					if(id[h].get_row() == pieces -1)
					{
						if(id[h].get_column() == feat && id[h].get_data() == 0)
						{
							row_num = id[h].get_row() +1; 
							converting << row_num;																				   partition1+= " "+ converting.str() + " ";	
							converting.str("");
							converting.clear(); 
						}	
						else if(id[h].get_column() == feat && id[h].get_data() ==1)
						{							
							row_num = id[h].get_row() +1; 
							converting << row_num;														     						  partition2+= " " + converting.str() + " ";
							converting.str(""); 
							converting.clear(); 
						}
					}
				}			

				
			}
				OutputFile << partition1 << endl;
				pid = pid + rand() % 20;
				cout << pid;
				OutputFile << pid;
				OutputFile << " ";
				OutputFile << partition2 << endl; 
			
		}
	}
	if (flag_w > 0)
	{
		cout << " Using Column " << feat+1 <<endl; 
	
		OutputFile.close(); 	
	}
	else
	{
		cout << "No Changes Necessary " << endl; 	
		return 0; 
	}
						
	inputFile.open(out_f.c_str());

	cout << "Contents of Partition File : " << endl; 
	while(getline(inputFile,read))
	{
		cout << read << endl; 
	
	}	

	inputFile.close(); 

return 0; 
}
