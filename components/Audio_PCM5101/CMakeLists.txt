idf_component_register(
	    SRCS "Audio_PCM5101.cpp"  # List all .cpp or .c files here
	        INCLUDE_DIRS "."          # This tells main where to find the .h files
		    REQUIRES driver 
	    )
