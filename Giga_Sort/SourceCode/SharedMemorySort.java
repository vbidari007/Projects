package vishwanath;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.ListIterator;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;
import java.util.logging.Logger;

/**
 * @author : Vishwanath Bidari
 * CS 553 Cloud Computing Assignment2
 * 
 * Goal is to implement Shared memory external sorting 
 * 
 */

/**
 * 
 * sort class implements sorting of split Files
 *
 */
class Sort implements Runnable {
    
    private File file;
       
    
    public Sort(File file) {
        this.file = file;
        
    }
    /**
     * 
     * Implements merge sort with n log n performance
     * @param src : source array
     * @param dest: sorted destination array
     * @param low : array low index
     * @param high : array high index
     */
    @SuppressWarnings("unchecked")
	public static void MergeSort(String[] src, String[] dest, int low, int high)
    {
    	int length = high - low;
    	 
		        int dLow  = low;    
    	        int dHigh = high;	


    	        int mid = (low + high)/2;    	

    	        MergeSort(dest, src, low, mid);    	

    	        MergeSort(dest, src, mid, high);
    	        for(int i = dLow, p = low, q = mid; i < dHigh; i++) {

    	        if (q >= high || p < mid)  	

    	        	   dest[i] = src[p++];    	       
 	        	  else
    	                   dest[i] = src[q++];    	     

    	        	   }
    }
    /**
     * Implements sorting with multi threading
     */
    public void run() {
      
        try {
        	//Get thread Id
        	 long threadId = Thread.currentThread().getId();
        	 String[] src;
        	 String[] dest;
        	 
        	 System.out.println("Thread: "+threadId+" working on file "+this.file.getName());
        	 FileReader fr=new FileReader(file);
                
            BufferedReader br = new BufferedReader(fr);
            List<String> ChunkData =  new ArrayList<String>();
            String line = "";
            while(line != null) {
               
                while( (line = br.readLine()) != null) { 
                	ChunkData.add(line);
                  
                }              
                
            }
            			
            src= new String[ChunkData.size()];
            dest=new String[ChunkData.size()];
            ChunkData.toArray(src);
            MergeSort(src,dest,0,src.length);
            ListIterator i = ChunkData.listIterator();

                 for (int j=0; j<dest.length; j++) {
                   i.next();
                    i.set(dest[j]);
       
                     }
                 FileWriter fw=new FileWriter(file);
            
            BufferedWriter bw = new BufferedWriter(fw);
            
                for(String r : ChunkData) {
                    bw.write(r);
                 
                    bw.newLine();
              
            
                bw.close();
            }
            ChunkData.clear();
            System.out.println("Thread: "+threadId+" done sorting on file "+this.file.getName());
        	
        } catch (IOException e) {
        }
        
      
    }
}

public class SharedMemorySort {
	static List<File> SplitFiles;
	
	/**
	 * 
	 * @param path : Input File path
	 * @param cmp  : String Comparator
	 * @throws IOException
	 */
	public static void ReadInputFileAndSplit(String path) throws IOException
	{
		SplitFiles=new ArrayList<File>();
		long chunksize=((Runtime.getRuntime().freeMemory())/8);
		FileReader fr=new FileReader(path);
		BufferedReader br=new BufferedReader(fr);
		List<String> ChunkData=new ArrayList<String>();
		long size=0;
		String data;
		boolean eof=true;
		while(eof)
		{
			data=br.readLine();
			if(data==null)
			{
				eof=true;
				//return ;
			}
					
			if(size < chunksize)
			{
				ChunkData.add(data);
				//size=data.length()*2;
			}
			else
			{
				  File tmpfile= File.createTempFile("SplitFile", "tmp",new File("/"));
			        tmpfile.deleteOnExit();
			        FileWriter fw=new FileWriter(tmpfile);
			        BufferedWriter bf=new BufferedWriter(fw);
			        for (String string : ChunkData) {
			        	bf.write(string);
			        	bf.write("\n");
			        	bf.close();
						
					}
			        ChunkData.clear();
			        size=0;
			        SplitFiles.add(tmpfile);
			}
		}
		
		return;
	}
	/**
	 * Merges the sorted split files
	 */
	
	public static void MergeSplitFiles(String path)
	{
		try {
			
			FileWriter fw=new FileWriter(path);
			BufferedWriter br=new BufferedWriter(fw);
			for (File f:SplitFiles) {
				FileReader sfw=new FileReader(f);
				BufferedReader sbr=new BufferedReader(sfw);	
				while(sbr.readLine()!=null)
				{
					String data=sbr.readLine();
					br.write(data);
					br.write("\r\n");
				}
				
				
			}
			br.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		
		return;
	}
	
	
	
	/**
	 * 
	 * @param args 
	 * Read Input file path ,Output file path and Number of threads from command line
	 * @throws IOException 
	 */
	public static void main(String[] args) throws IOException {
		    String InputFilePath = args[0];
	        String OutputFilePath = args[1];
	        int NumThreads=Integer.parseInt(args[2]);
	        System.out.println("\n=========================== Shared Memory Sorting ======================= \n");
	        System.out.println("\nTotal Threads: "+String.valueOf(NumThreads));
	        //Create thread pool
	        ExecutorService executor = Executors.newFixedThreadPool(NumThreads);
	         
	          // Start time for Shared memory sorting      
	         long start=System.currentTimeMillis();
	         ReadInputFileAndSplit(InputFilePath);
	        //Assign Tasks to Thread pool
	         for (File file : SplitFiles) {
	         	
	         	 executor.submit(new Sort(file));
	 		}
	         executor.shutdown();
	         
	        
	         
	         try {
	             executor.awaitTermination(1, TimeUnit.DAYS);
	         } catch (InterruptedException e) {
	         }
	         MergeSplitFiles(OutputFilePath);
	         
	         //End time for Shared memory sorting
	         long end=System.currentTimeMillis();
	         
	         //Elapse Time for Shared memory sorting
	         System.out.println("\nElapse Time in ms: "+String.valueOf((end-start)));
	         
        
    }

}
