# -*- coding: utf-8 -*-
"""
Created on Mon Feb 10 16:54:05 2020

@author: chris
"""
from collections import Counter
import matplotlib.pyplot as plt, numpy as np

def unique_enigma_configurations(letters ):
    
    return 0

def make_configuration_grid(letters, rotors, notches_1, notches_2):
    return 0

def make_standard_rotor_positions(letters, rotors, notches_1, notches_2):
    print("\rmaking rotor positions...", end="")
    rotor_positions=[]
    current_rotor_position=[0]*rotors #might not be correct!
    #step until we get something that is probably a possible configuration
    for i in range(pow(letters, rotors)):
        #print(i)
        current_rotor_position=step(letters, current_rotor_position, notches_1, notches_2)
    #now set actual initial
    initial_rotor_position=current_rotor_position
    rotor_positions.append(initial_rotor_position)
    current_rotor_position=step(letters, initial_rotor_position, notches_1, notches_2)
    #step untial we hit initial again
    while(current_rotor_position!=initial_rotor_position):
        rotor_positions.append(current_rotor_position)
        current_rotor_position=step(letters, current_rotor_position, notches_1, notches_2)
        #print(current_rotor_position)
    print("\rmade rotor positions      ", end="")
    return rotor_positions
  
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
    
def step(letters, rotor_position, notches_1, notches_2):
    rotors=len(rotor_position)
    out=[(rotor_position[0]+1)%letters]
    pos_p=rotor_position[0]
    for pos, i in zip(rotor_position[1:], range(rotors-1)):
        setting=pos
        if ((pos_p==notches_1[i]) or (pos_p==notches_2[i])) or (i+1<rotors-1 and ((pos==notches_1[i+1]) or (pos==notches_2[i+1]))):
            setting=(setting+1)%letters
        out+=[setting]
        pos_p=pos
    return out

def histogram_of_uniques(letters, rotor_positions, ring_settings, concatenations, notches_1, notches_2):
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

def uniques(rotors, over_letters, notches_1, notches_2):
    unique_counts=[]
    compression_counts=[]
    for letters in over_letters:
        previous_uniques=0
        unique_count=[]
        compression_count=[]
        rotor_positions=make_standard_rotor_positions(letters, rotors, notches_1, notches_2)
        ring_settings=make_standard_ring_settings(letters, rotors)
        for concatenations in range(1, pow(letters, rotors+1)):
            uniques=len(histogram_of_uniques(letters, rotor_positions, ring_settings, concatenations, notches_1, notches_2).keys())
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
    
def count_rotor_positions_with_duplicates(rotors, letters):
    duplicate_counter={}
    
    #we only look at double notching
    notch_permutations=[]
    for i in range(pow(letters, rotors)): #rotors indexes, letters possible values
        number=i
        notches=[0]*rotors
        for j in range(rotors):
            notches[j]=number%letters
            number=int((number-number%letters)/letters)
        notch_permutations.append(notches)
    for notches_1 in notch_permutations:
        for notches_2 in notch_permutations:
            #dissalow consecutive notches in same rotor 
            break_me=False
            for notch_1, notch_2, rotor in zip(notches_1, notches_2, range(rotors)):
                if (notch_1+1)%letters==notch_2 or notch_1==(notch_2+1)%letters:
                    break_me=True
            if break_me:
                break
            
            count=len(make_standard_rotor_positions(letters, rotors, notches_1, notches_2))
            #notches_string=' '.join([str(n1)+","+str(n2) for n1, n2 in zip(notches_1, notches_2)])
            #print(notches_string + " rotor_positions: "+str(count), end="")
            #check for duplicates, assign to a dictoinary based on which are duplicate
            duplicates=[False]*rotors
            for notch_1, notch_2, rotor in zip(notches_1, notches_2, range(rotors)):
                if notch_1==notch_2:
                    #print("  "+"             "*rotor+"duplicate "+str(rotor), end="")    
                    duplicates[rotor]=True
                    #print()
            #get hash of duplicates:
            duplicates_hashed=0
            for d in duplicates:
                duplicates_hashed*=2
                duplicates_hashed+=d
            if duplicates_hashed in duplicate_counter.keys():
                if count in duplicate_counter[duplicates_hashed].keys():
                    duplicate_counter[duplicates_hashed][count]+=1
                else:
                    duplicate_counter[duplicates_hashed][count]=0
            else:
                duplicate_counter[duplicates_hashed]={count:1}
    #pretty print the dictionary
    for key in duplicate_counter:
        #unhash the key
        duplicates=[False]*rotors
        number=key
        for r in range(rotors):
            duplicates[r]=str(number%2)
            number=int((number-number%2)/2)
        print(','.join(duplicates)+":")
        print(duplicate_counter[key])
            
        
def count_rotor_positions(rotors, letters, max_notches=2, printmode=""):
    notch_counter={}
    
    #we only look at double notching
    notch_permutations=[]
    for i in range(pow(letters, rotors)): #rotors indexes, letters possible values
        number=i
        notches=[0]*rotors
        for j in range(rotors):
            notches[j]=number%letters
            number=int((number-number%letters)/letters)
        notch_permutations.append(notches)
    for notches_1 in notch_permutations:
        for notches_2 in notch_permutations:
            #dissalow consecutive notches in same rotor 
            break_me=False
            for notch_1, notch_2, rotor in zip(notches_1, notches_2, range(rotors)):
                if (notch_1+1)%letters==notch_2 or notch_1==(notch_2+1)%letters:
                    break_me=True
            if break_me:
                break
            
            count=len(make_standard_rotor_positions(letters, rotors, notches_1, notches_2))
            #notches_string=' '.join([str(n1)+","+str(n2) for n1, n2 in zip(notches_1, notches_2)])
            #print(notches_string + " rotor_positions: "+str(count), end="")
            #check for duplicates, assign to a dictoinary based on which are duplicate
            notch_count=[0]*rotors
            for notch_1, notch_2, rotor in zip(notches_1, notches_2, range(rotors)):
                if notch_1==notch_2:
                    notch_count[rotor]=1
                else:
                    notch_count[rotor]=2
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
            print(notch_counter[key])
    return
    #print(duplicate_counter)
    
#only works if notches are at least one position apart
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
