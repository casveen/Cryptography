# -*- coding: utf-8 -*-
"""
Created on Mon Feb 10 16:54:05 2020

@author: chris
"""
from collections import Counter
import matplotlib.pyplot as plt, numpy as np
import sys


def make_standard_rotor_positions(letters, rotors, all_notches):
    print("\rmaking rotor positions...", end="")
    rotor_positions=[]
    current_rotor_position=[0]*rotors #might not be correct!
    #step until we get something that is probably a possible configuration
    for i in range(pow(letters, 1)):
        #print(i)
        current_rotor_position=step(letters, current_rotor_position, all_notches)
    #now set actual initial
    initial_rotor_position=current_rotor_position
    rotor_positions.append(initial_rotor_position)
    current_rotor_position=step(letters, initial_rotor_position, all_notches)
    #step untial we hit initial again
    while(current_rotor_position!=initial_rotor_position):
        rotor_positions.append(current_rotor_position)
        current_rotor_position=step(letters, current_rotor_position, all_notches)
        #print(current_rotor_position)
    print("\rmade rotor positions      ", end="")
    return rotor_positions

def count_standard_rotor_positions(letters, rotors, all_notches):
    current_rotor_position=[0]*rotors #might not be correct!
    #step until we get something that is probably a possible configuration
    for i in range(pow(letters, 1)):
        #print(i)
        current_rotor_position=step(letters, current_rotor_position, all_notches)
    #now set actual initial
    initial_rotor_position=current_rotor_position
    current_rotor_position=step(letters, initial_rotor_position, all_notches)
    #step untial we hit initial again
    count=1
    while(current_rotor_position!=initial_rotor_position):
        count+=1
        current_rotor_position=step(letters, current_rotor_position, all_notches)
    return count
  
def make_standard_ring_settings(letters, rotors):
    print("\rmaking ring settings...", end="")
    ring_settings=[]
    for i in range(pow(letters, rotors)):
        number=i
        ring_setting=[0]*rotors
        for j in range(rotors):
            ring_setting[j]=number%letters
            number=int((number-number%letters)/letters)
        ring_settings.append(ring_setting)
    print("\rmade ring settings        ", end="")
    return ring_settings

def make_configuration(letters, rotor_positions, rotor_position, ring_setting, concatenations):
    configurations=[]
    rotors=len(rotor_positions[0])
    for i in range(concatenations):
        configuration=[]
        for j in range(rotors):
            #print_progress(i*rotors+j, concatenations*rotors, 50, "\r"+52*" ", " ")
            configuration+=[(rotor_positions[(rotor_position+i)%len(rotor_positions)][j]-ring_setting[j]+letters)%letters]
        configurations.append(configuration)
    return configurations

def hash_configurations(configurations, letters):
    hash_out=0
    rotors=len(configurations[0])
    for configuration in configurations:
        hash_out*=pow(letters, rotors)
        hash_out+=hash_configuration(configuration, letters)
    return hash_out    
    
def hash_configuration(configuration, letters):
    #hash a single config
    hash_out=0
    for i in configuration:
        hash_out*=letters
        hash_out+=i
    return hash_out
    
def step(letters, rotor_position, all_notches):
    rotors=len(rotor_position)
    out=[(rotor_position[0]+1)%letters]
    pos_p=rotor_position[0]
    for pos, i in zip(rotor_position[1:], range(rotors-1)):
        setting=pos
        stepping=False
        for current_notch, next_notch in zip(all_notches[i], all_notches[i+1]):
            if (pos_p==current_notch)  or (i+1<rotors-1 and pos==next_notch):
                stepping=True
                break
        out+=[(pos+stepping)%letters]
        pos_p=pos
    return out

def histogram_of_uniques(letters, rotor_positions, ring_settings, concatenations):
    rotor_position_count=len(rotor_positions)
    ring_setting_count=len(ring_settings)
    rotors=len(rotor_positions[0])
    histogram={}
    for rp in range(rotor_position_count):
        for rs in range(ring_setting_count):
            print_progress(rp*ring_setting_count+rs, rotor_position_count*ring_setting_count, 100, "\r", " ")
            ring_setting=ring_settings[rs]
            configurations=make_configuration(letters, rotor_positions, rp, ring_setting, concatenations)         
            hash_conf=hash_configurations(configurations, letters)
            if hash_conf in histogram.keys():
                histogram[hash_conf]+=1
            else:
                histogram[hash_conf]=0
    print("\r", end="")
    return histogram

def print_progress(progress, total, width, start="", end=""):
    length=int(progress/total*width)
    print(start+"["+"#"*length+" "*int(width-1-length)+"]", end=end)

def uniques(rotors, over_letters, nall_notches):
    unique_counts=[]
    compression_counts=[]
    for letters in over_letters:
        previous_uniques=0
        unique_count=[]
        compression_count=[]
        rotor_positions=make_standard_rotor_positions(letters, rotors, all_notches)
        ring_settings=make_standard_ring_settings(letters, rotors)
        for concatenations in range(1, pow(letters, rotors+1)):
            uniques=len(histogram_of_uniques(letters, rotor_positions, ring_settings, concatenations).keys())
            if uniques==previous_uniques:
                break
            print("rotors: %d, letters %2d, concat %2d:     uniques %6d,    diff %6d,     compression %f"%(rotors, letters, concatenations, uniques, uniques-previous_uniques, uniques/(len(rotor_positions)*len(ring_settings))))
            previous_uniques=uniques
            unique_count+=[uniques]
            compression_count+=[uniques/(len(rotor_positions)*len(ring_settings))]
        print()
        unique_counts.append(unique_count)
        compression_counts.append(compression_count)
    return unique_counts, compression_counts
    
def plot_counts(unique_counts, normalize_x=True, normalize_y=False):
    for count in unique_counts:
        count=np.array(count)
        if normalize_y:
            count=count/np.max(count)
        if normalize_x:
            x=np.linspace(0,1,len(count))
            plt.plot(x, count)
        else:
            plt.plot(count)
    plt.show()
    
def plot_compression(compression_counts):
    for count in compression_counts:
        plt.plot(count)
    plt.show()



def permutations_of_notches(rotors, letters, max_notches, max_amount): #without consecutives
    permutations_out=[]
    possible_notches=[]
    notch_counts_hit=[0]*max_notches
    for i in range(pow(letters, max_notches)):
        possible_notch=[0]*max_notches
        number=i
        for j in range(max_notches):
            possible_notch[j]=int(number%letters)
            number=(number-number%letters)/letters
        #check if valid
        unique_notches=possible_notch#Counter(possible_notch).keys()
        #print(unique_notches)
        valid=True
        for ii in range(len(unique_notches)):
            for jj in range(ii+1,len(unique_notches)):
                if (unique_notches[ii]+1)%letters==unique_notches[jj] or (unique_notches[jj]+1)%letters==unique_notches[ii]:
                    valid=False
                    break
        #get number of notches
        if valid:
            notches_count=len(Counter(unique_notches))
            if notch_counts_hit[notches_count-1]>=max_amount:
                valid=False
            else:
                notch_counts_hit[notches_count-1]+=1
        #append if valid
        if valid:
            possible_notches.append(possible_notch)
    #make one "all_notches", add to permutations
    return permutations_of(possible_notches, rotors)

def permutations_of(array, n): #repeats
    print("\rpermuting"+"-"*n+"#"+"          ", end="")
    out=[]
    if n>=2:
        for element in array:
            for new_element in permutations_of(array, n-1):
                #print(new_element)
                out.append([element]+new_element)    
    elif n==1:
        for element in array:
            out.append([element])   
    return out


#count rotor positions for given rotors and letters. notch positions dos not matter, but position and non-consecutive on a rotor does
#unique amount of notches=1 ensures that not absolutely all permutations are counted, which can take a lot of time but is most thorough
#ususally all permutations are counted, but if you are sure that all notchings 
def count_rotor_positions(rotors, letters, max_notches=2, printmode="", unique_amount_of_notches=sys.maxsize):
    notch_counter={}
    all_notches=permutations_of_notches(rotors, letters, max_notches, unique_amount_of_notches)
    #print(all_notches)
    for i,notches in enumerate(all_notches): 
        #print(notches)
        print_progress(i, len(all_notches), 100, "\r")
        #print(notches)
        count=count_standard_rotor_positions(letters, rotors, notches)
            #notches_string=' '.join([str(n1)+","+str(n2) for n1, n2 in zip(notches_1, notches_2)])
            #print(notches_string + " rotor_positions: "+str(count), end="")
            #check for duplicates, assign to a dictoinary based on which are duplicate
        #count notches per rotor
        notch_count=[0]*rotors
        for rotor in range(rotors):
            notch_count[rotor]=len(Counter(notches[rotor]).keys())
        #get hash of notches:
        notches_count_hashed=0
        for d in notch_count[::1]:
            notches_count_hashed*=max_notches
            notches_count_hashed+=(d-1)
        if notches_count_hashed in notch_counter.keys():
            if count in notch_counter[notches_count_hashed].keys():
                notch_counter[notches_count_hashed][count]+=1
            else:
                notch_counter[notches_count_hashed][count]=0
        else:
            notch_counter[notches_count_hashed]={count:1}
    #pretty print the dictionary
    print()
    if printmode=="counts":
        for key in range(pow(max_notches,rotors)):
            for keykey in notch_counter[key].keys():
                print(keykey, end=" ")
            print()
    elif printmode=="notches":
        for key in range(pow(max_notches,rotors)):
            #unhash the key
            notches=[0]*rotors
            number=key
            for r in range(rotors):
                notches[r]=str(number%max_notches+1)
                number=int((number-number%max_notches)/max_notches)
            print('\t'.join(notches), end="\n")
    else:
        for key in range(pow(max_notches,rotors)):
            #unhash the key
            notches=[0]*rotors
            number=key
            for r in range(rotors):
                notches[r]=str(number%max_notches+1)
                number=int((number-number%max_notches)/max_notches)
            print('|'.join(notches)+":", end=" ")
            if key in notch_counter.keys():
                print(notch_counter[key])
    return
    #print(duplicate_counter)
    
#cannot handle if number of notches does not divide letters
#only works if notches are at least one position apart
#return amount of rotor positions
def rotor_position_count(rotors, letters, all_notches):
    count=letters
    #first rotor has no influence
    for rotor, notches in enumerate(all_notches[1:-1:]):
        #count notches
        notch_count=len(Counter(notches).keys())
        count*=(letters-notch_count)/notch_count
    #last rotor
    count*=letters/len(Counter(all_notches[-1]).keys())
    return count


#len(histogram_of_uniques(5, 3, 5, notches_1, notches_2).keys())
            
#count_rotor_positions(2,3) all equal        
#count_rotor_positions(2,4)   only rotor 1 notches affect
#count_rotor_positions(2,5) all equal
#count_rotor_positions(2,6)   only rotor 1 notches affect
