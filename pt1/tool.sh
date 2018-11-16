filename=""
if [ $# -gt 0 ]; then								#check if arguments exist

	##########Check for -f
	for (( i=1; i<$#; i++ ))
	do
		eval k="\$$(( i+1 ))"					#get the next argument into k on each iteration
		if [ ${!i} == '-f' ] && [ -r "$k" ]; then			#file exists and is readable
			filename=$k
		fi
	done

	if [ -z $filename ]; then						#Arguments exist, file error
		echo "Could not read from $filename. Check the name or permissions of the file!"
		exit
	fi
#read -p "File opened..."
#	if [ $# -eq 2 ]; then    						#if arguments count exactly 2
#		sed '/([ ]|[\t])*#/d' $filename       				#A part of assignment
#	fi

	##########Check for -id
#read -p "Checking for -id..."
	for (( i=1; i<$#; i++ ))
        do
		eval k="\$$(( i+1 ))"
                if [ ${!i} == '-id' ]; then               			#-id exists, B part of assignment
                        awk -v studentID="$k" -F "|" 'BEGIN {}
                                                NF==8 && !/([ ]|[\t])*#/ && $1==studentID {print $3" "$2" "$5}
                                                END {}' $filename
			exit
                fi
        done
#read -p "Checking for firstnames..."
	##########Check for --firstnames
	for (( i=1; i<=$#; i++ ))
        do
                if [ ${!i} == '--firstnames' ]; then
                        awk -F "|" 'NF==8 && !/([ ]|[\t])*#/ {print $3}' $filename | sort | uniq
			exit
                fi
        done
#read -p "Checking for lastnames..."
	##########Check for --lastnames
        for (( i=1; i<=$#; i++ ))
        do
                if [ ${!i} == '--lastnames' ]; then
                        awk -F "|" 'NF==8 && !/([ ]|[\t])*#/ {print $2}' $filename | sort | uniq
			exit
                fi
        done
#read -p "Checking for browsers..."
	##########Check for --browsers
        for (( i=1; i<=$#; i++ ))
        do
                if [ ${!i} == '--browsers' ]; then
                        awk -F "|" 'NF==8 && !/([ ]|[\t])*#/ {print $8}' $filename | sort | uniq -c | awk -F " " '{if(length($3)>1) {browser=$2" "$3} else {browser=$2} if(length($4)>1) {browser=$2" "$3" "$4}  print browser" "$1}'
			exit
                fi
        done
#read -p "Checking for edit..."
	##########Check for --edit
	n=$#
	n=$(( n-2 ))
        for (( i=1; i<n; i++ ))
        do
		eval k="\$$(( i+1 ))"
		eval l="\$$(( i+2 ))"
		eval m="\$$(( i+3 ))"
		if [ ${!i} == '--edit' ] && [ $l -ge 2 ] && [ $l -le 8 ]; then 			 #condition for G
					record_file=$( awk -v studentID="$k" -v col="$l" -v val="$m" -F "|" 'BEGIN {}
                                                {
                                                if (NF==8 && !/([ ]|[\t])*#/ && $1==studentID) {
                                                        $col=val;
                                                        print $1"|"$2"|"$3"|"$4"|"$5"|"$6"|"$7"|"$8
                                                        }
                                                else {print $0}
                                                }
                                                END {}' $filename )
					echo "$record_file"  > "$filename"
					exit
		fi
	done
#read -p "Checking for born-since and born-until..."
	##########Check for --born-since and/or --born-until
	for (( i=1; i<$#; i++ ))
        do
		eval k="\$$(( i+1 ))"
		if [ ${!i} == '--born-since' ]; then
			for (( j=1; j<$#; j++ ))
			do
				eval l="\$$(( j+1 ))"
				if [ ${!j} == '--born-until' ]; then
					#####Both --born-since and --born-until exist
					awk -v dSince="$k" -v dUntil="$l" -F "|" 'BEGIN{
                                        split(dSince, a, "-")
                                        split(dUntil, b, "-") }
                                        !/([ ]|[\t])*#/ {
                                        split($5, tmp, "-")
                                        if (tmp[1]>a[1] && tmp[1]<b[1]) {print $0}
                                        else if (tmp[1]>a[1] && tmp[1]==b[1]) {
                                                if (tmp[2]<b[2]) {print $0}
                                                else if (tmp[2]==b[2]) {
                                                        if (tmp[3]<=b[3]) {print $0}
                                                        }
                                                }
                                        else if (tmp[1]==a[1] && tmp[1]<b[1]) {
                                                if (tmp[2]>a[2]) {print $0}
                                                else if (tmp[2]==a[2]) {
                                                        if (tmp[3]>=a[3]) {print $0}
                                                        }
                                                }
                                        else if (tmp[1]==a[1] && tmp[1]==b[1]) {
                                                if (tmp[2]>a[2] && tmp[2]<b[2]) {print $0}
                                                else if (tmp[2]==a[2] && tmp[2]<b[2]) {
                                                        if (tmp[3]>=a[3]) {print $0}
                                                        }
                                                else if (tmp[2]>a[2] && tmp[2]==b[2]) {
                                                        if (tmp[3]<=b[3]) {print $0}
                                                        }
                                                else if (tmp[2]==a[2] && tmp[2]==b[2] && tmp[3]>=a[3] && tmp[3]<=b[3]) {print $0}
                                                }
                                        }
                                        END{}' $filename
					exit
				fi
			done
			#####Only --born-since exists
			awk -v dSince="$k" -F "|" 'BEGIN{
                                        split(dSince, a, "-")}
                                        !/([ ]|[\t])*#/ {
                                        split($5, tmp, "-")
                                        if (tmp[1]>a[1]) {print $0}
                                        else if (tmp[1]==a[1]) {
                                                if (tmp[2]>a[2]) {print $0}
                                                else if (tmp[2]==a[2] && tmp[3]>=a[3]) {print $0}
                                                }
                                        }
                                        END{}' $filename
			exit
		fi
	done

	##########Only --born-until exists
        for (( i=1; i<$#; i++ ))
        do
		eval k="\$$(( i+1 ))"
                if [ ${!i} == '--born-until' ]; then
			awk -v dUntil="$k" -F "|" 'BEGIN{
                                        split(dUntil, b, "-")}
                                        !/([ ]|[\t])*#/ {
                                        split($5, tmp, "-")
                                        if (tmp[1]<b[1]) {print $0}
                                        else if (tmp[1]==b[1]) {
                                                if (tmp[2]<b[2]) {print $0}
                                                else if (tmp[2]==b[2] && tmp[3]<=b[3]) {print $0}
                                                }
                                        }
                                        END{}' $filename
			exit
		fi
	done
#read -p "Only file argument given!..."
	sed '/^#/d' $filename         #A part of assignment
	exit
else
	echo "serial_number"
fi
exit


