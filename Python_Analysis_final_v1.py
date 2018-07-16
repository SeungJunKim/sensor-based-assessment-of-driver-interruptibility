
from numpy import genfromtxt
from numpy import hstack
from numpy import ravel

import numpy
# remove - from sklearn import svm
# remove - from sklearn import tree

#from skll import Learner

from sklearn.naive_bayes import GaussianNB, MultinomialNB, BernoulliNB

from sklearn.svm import SVC
from sklearn.svm import NuSVC
from sklearn.svm import SVR
from sklearn.svm import NuSVR
from sklearn.svm import LinearSVC

from sklearn.tree import DecisionTreeClassifier
from sklearn.ensemble import RandomForestClassifier

from sklearn import cross_validation
from sklearn.cross_validation import StratifiedKFold

from sklearn.cross_validation import train_test_split
from sklearn.metrics import confusion_matrix
from sklearn.metrics import classification_report
from sklearn.metrics import roc_curve, auc

#import pylab as pl

import os.path

save_path_home ='C:\Users\SeungJun\Dropbox\[CHI 2015] Driving Capture Study\[02] Analysis\Weka and SPSS outputs finals\Final Source\SPSS'
save_path_office ='C:\Users\Anind_Tobii\Dropbox\[CHI 2015] Driving Capture Study\[02] Analysis\Weka and SPSS outputs finals\Final Source\SPSS'
save_path_laptop ='C:\Users\sjunikim\Dropbox\[CHI 2015] Driving Capture Study\[02] Analysis\Weka and SPSS outputs finals\Final Source\SPSS'
name_of_file = raw_input("What is the name of the file: ")

completeName = os.path.join(save_path_laptop, "SJ_Python_output_Test_Individual"+name_of_file+".txt")

f = open(completeName, "w")

string_to_print = os.path.abspath(__file__) + '\n\nLoading....'
print(string_to_print)
f.write(string_to_print)

#file_name = '\FInal_LINT_filtered_final_51.csv'
#file_name = '\FInal_LINT_filtered_final_51_annot_aside.csv'
file_name = '\FInal_LINT_filtered_final_51_annot_and_plusF_aside.csv'
#file_name = '\FInal_LINT_filtered_final_51_only_raw_data.csv'


#my_data = genfromtxt(save_path_home+file_name, delimiter=',', dtype="|S60")
my_data = genfromtxt(save_path_laptop+file_name, delimiter=',', dtype="|S60")

f.write(file_name+"\n\n")


# CSV source info - 58195+1 rows x 146 columns

n_instance = len(my_data); # number of rows
print("n_instance: %d\n" %n_instance)
print("shape: %s" %[my_data.shape])

first_row_num_to_load = 1


s_X_resources = 'All'       #    All/Basic_only/Plus_only/OBD_only/YEI_only/BH_only
  
s_y_target ='int_states'    #    int_states/events/drv_activities


import scipy

if s_X_resources == 'All':
    #X_selected_missing = my_data[first_row_num_to_load-1:n_instance,3-1:25]#143]#138]#143] #Col_index] # 
    X_selected_missing = my_data[first_row_num_to_load-1:n_instance,1-1:143]
    #X_selected_missing = my_data[first_row_num_to_load-1:n_instance,1-1:138]
'''
if s_X_resources == 'Except_X_OUTSIDE':
    X_selected_missing = hstack((X_subject_ID,X_session_ID,X_OBD,X_YEI,X_BH))

'''


Header_names = X_selected_missing[0,:]

print('Header Names (of Selected Sensor feature)\n'+str(Header_names))
f.write('Header Names (of Selected Sensor feature)\n'+str(Header_names)+'\n\n')

X_selected_missing = X_selected_missing[1:,:]
X_selected_missing = X_selected_missing.astype(numpy.float) 

'''
from sklearn.preprocessing import Imputer
imp = Imputer(missing_values=-999, strategy='mean', axis=0) #imp = Imputer(missing_values='NaN', strategy='mean', axis=0)
imp.fit(X_selected_missing)
X_final = imp.transform(X_selected_missing)
'''

X_final = X_selected_missing

'''
y_capacity = ravel(my_data[2-1:n_instance, 129-1:129])     # consider - 129-1:129
y_event = ravel(my_data[2-1:n_instance, 130-1:130])        # consider - 130-1:130
y_activity = ravel(my_data[2-1:n_instance, 131-1:131])     # consider - 131-1:131
'''
#y_capacity = my_data[2-1:n_instance, 129-1:129]     # consider - 129-1:129
#y_capacity = my_data[2-1:n_instance, 160-1:160]     # consider - 129-1:129

y_capacity = my_data[2-1:n_instance, 145-1:145]     # consider - 129-1:129
#y_event = my_data[2-1:n_instance, 130-1:130]        # consider - 130-1:130
#y_activity = my_data[2-1:n_instance, 131-1:131]     # consider - 131-1:131

#index_col = my_data[2-1:n_instance, 161-1:161] 
#timestamp_col = my_data[2-1:n_instance, 162-1:162] 
index_col = my_data[2-1:n_instance, 146-1:146] 
timestamp_col = my_data[2-1:n_instance, 147-1:147] 


if s_y_target == 'int_states':
    y_final = y_capacity

#if s_y_target == 'events':
#    y_final = y_event

#if s_y_target == 'drv_activities':
#    y_final = y_activity



print([X_final.shape]+[y_final.shape])



s_final_resources = '\nX resources:% s, y target: %s\n' % (s_X_resources,s_y_target)
print(s_final_resources)
f.write(s_final_resources)

string_to_print = 'shape_X_final (all subjects) - %s \nshape_y_final (all subjects) - %s \n\n' % ([X_final.shape],[y_final.shape])
print(string_to_print)
f.write(string_to_print)


subject_to_examine = 17# 5, 8, 9, 10, 11, 13, 14, 15, 19, 21, 23, 24, 25, 26, 17   #0 


class_labels_for_CR = ['INTERRUPTIBLE(PI+NO_HAND_DRIVE)', 'LESS_INTERRUPTIBLE(STEERING_ONLY+DRIVING_I+ONE_HAND_DRIVE_WITH_NO_PI)']

output_file_to_print = 'Results_sb%s_%dclass_%s.csv' % (subject_to_examine,len(class_labels_for_CR),class_labels_for_CR[0]) # print out output (truth, predicted)


# Test individuals by the model of the other else
if subject_to_examine==0:
    X_final_test = X_final # extract all rows with the first column 'subject_to_examine'
    y_final_truth = y_final # extract all rows with the first column 'subject_to_examine'
else:
    X_final_test = X_final[X_final[:, 0] == subject_to_examine,:] # extract all rows with the first column 'subject_to_examine'
    y_final_truth = y_final[X_final[:, 0] == subject_to_examine,:] # extract all rows with the first column 'subject_to_examine'
    index_col = index_col[X_final[:, 0] == subject_to_examine,:]
    timestamp_col = timestamp_col[X_final[:, 0] == subject_to_examine,:]

#if subject_to_examine!=0:
    #index_col = index_col[X_final[:, 0] == subject_to_examine,:]
    #timestamp_col = timestamp_col[X_final[:, 0] == subject_to_examine,:]    
#else:
    #index_col = index_col[X_final[:, 0] == subject_to_examine,:]
    #timestamp_col = timestamp_col[X_final[:, 0] == subject_to_examine,:]

#X_final_train = X_final[X_final[:, 0] != subject_to_examine,:] #X_final; # # extract all rows with the first column not 'subject_to_examine'
#y_final_train = y_final[X_final[:, 0] != subject_to_examine,:] #y_final; # # extract all rows with the first column not  'subject_to_examine'

### Working up to here!!
#string_to_print = 'Shape (all subjects): \nX_final - %s\ny_final - %s \n' % (X_final.shape,len(y_final))
string_to_print = 'Shape (all subjects): \nX_final - %s\ny_final - %s \n' % (X_final.shape,y_final.shape)
print(string_to_print)
f.write(string_to_print)

#string_to_print = 'Shape (sb%s): \nX_final_test - %s \ny_final_truth - %s \n' % (subject_to_examine, X_final_test.shape, len(y_final_truth))
string_to_print = 'Shape (sb%s): \nX_final_test - %s \ny_final_truth - %s \n' % (subject_to_examine, X_final_test.shape, y_final_truth.shape)
print(string_to_print)
f.write(string_to_print)

#string_to_print = 'Shape (except for sb%s):X_final_train - %s \ny_final_train - %s \n' % (subject_to_examine, X_final_train.shape, len(y_final_train))
#string_to_print = 'Shape (except for sb%s):\nX_final_train - %s \ny_final_train - %s \n' % (subject_to_examine, X_final_train.shape, y_final_train.shape)
#print(string_to_print)
#f.write(string_to_print)

print('index: %s', index_col.shape)

string_to_print = 'Shape (all subjects): \nX_final - %s\ny_final - %s \n' % (X_final.shape,y_final.shape)
print(string_to_print)
f.write(string_to_print)

string_to_print = 'Shape (sb%d): \nX_final_test - %s \ny_final_truth - %s \n' % (subject_to_examine, X_final_test.shape, y_final_truth.shape)
print(string_to_print)
f.write(string_to_print)

#string_to_print = 'Shape (except for sb%d):\nX_final_train - %s \ny_final_train - %s \n' % (subject_to_examine, X_final_train.shape, y_final_train.shape)
#print(string_to_print)
#f.write(string_to_print)

y_final = ravel(y_final)
#y_final_train = ravel(y_final_train)
y_final_truth = ravel(y_final_truth)

index_col = ravel(index_col)
timestamp_col = ravel(timestamp_col)


# Split the data into a training set and a test set
'''
print("X_final: %s" %[X_final.shape])
print("y_final: %s" %[y_final.shape])

f.write('\nX_final:'+str(X_final.shape))
f.write('\ny_final:'+str(y_final.shape)+'\n')
'''

# Choose a classifier
#GNB = GaussianNB()
#DTREE = DecisionTreeClassifier()
RFOREST = RandomForestClassifier()

final_y_pred =[[],[],[]] #  the number of [] = the number of classifiers
final_y_true =[[],[],[]] 
fold_index_col =[[],[],[]] 
fold_timestamp_col =[[],[],[]] 
#final_y_pred[0] = ravel(final_y_pred[0])
#final_y_pred[1] = ravel(final_y_pred[1])
final_y_pred[2] = ravel(final_y_pred[2])
final_y_true[2] = ravel(final_y_true[2])
fold_index_col[2] = ravel(fold_index_col[2])
fold_timestamp_col[2] = ravel(fold_timestamp_col[2])

n_folds_to_run = 10
remained_fold = n_folds_to_run

#kfold = cross_validation.KFold(len(y_final), n_folds=n_folds_to_run)
kfold = cross_validation.StratifiedKFold(y_final_truth, n_folds=n_folds_to_run)

fold_score_array = [[None] * n_folds_to_run for x in range(3)] # 3 classifiers
fold_acc_array = [[None] * n_folds_to_run for x in range(3)] # 3 classifiers
feature_importance_array = [[None] * n_folds_to_run for x in range(3)] # 3 classifiers

feature_importance_sum_1 = numpy.zeros(Header_names.shape) # initialze with the shape of header array
feature_importance_sum_2 = numpy.zeros(Header_names.shape) # initialze with the shape of header array

from scipy.stats import itemfreq

for train_indices, test_indices in kfold:    
 
    # Random Forest
    
    #s_fold_results = '\n\nPredicted by Random Forest (fold: %d/%d) - %s \nAccuracy (Random Forest): %s' % (n_folds_to_run-remained_fold+1, n_folds_to_run, RFOREST.fit(X_final[train_indices], y_final[train_indices]).predict(X_final[test_indices]),RFOREST.fit(X_final[train_indices], y_final[train_indices]).score(X_final[test_indices], y_final[test_indices]))
    print('\nTrain Indices: %s' %train_indices)
    print('\nTest Indices: %s' %test_indices)
    
    class0 = float(itemfreq(y_final_truth[train_indices])[0][1])
    class999 = float(itemfreq(y_final_truth[train_indices])[1][1])
    len_train = float(len(y_final_truth[train_indices]))
    
    print ("\nclass0: %f, class999: %f, total:%f" %(class0, class999, len_train))
    s_weights = numpy.array([class0/len_train if i == '999' else class999/len_train for i in y_final_truth[train_indices]]) # 0 = PROVED_PI_AVAILABLE (29% in actual datasets), 999 = UNPROVED_PI_AVAILABLE (70% in actual datasets)
    
    MODEL=RFOREST.fit(X_final_test[train_indices], y_final_truth[train_indices], sample_weight=s_weights)
    #MODEL=RFOREST.fit(X_final[train_indices], y_final[train_indices], sample_weight=s_weights)
    #MODEL=RFOREST.fit(X_final[train_indices], y_final[train_indices])
    
    predicted_y = MODEL.predict(X_final_test[test_indices])
    #predicted_y = MODEL.predict(X_final[test_indices])
    
    fold_score_array[2][int(n_folds_to_run-remained_fold)]= MODEL.score(X_final_test[test_indices], y_final_truth[test_indices])
    s_fold_results = '\nPredicted by Random Forest (fold: %d/%d) - %s \nScore (Random Forest): %s' % (n_folds_to_run-remained_fold+1, n_folds_to_run, predicted_y, fold_score_array[2][int(n_folds_to_run-remained_fold)])
   
    feature_importance = RFOREST.feature_importances_   
    feature_importance = 100.0 * feature_importance #feature_importance = 100.0 * (feature_importance / feature_importance.max())
    feature_importance_array[2][int(n_folds_to_run-remained_fold)] = feature_importance
    
    feature_importance_sum_2 += feature_importance
     
    print(s_fold_results)
    f.write(s_fold_results)
    
    final_y_pred[2] = numpy.concatenate((final_y_pred[2], predicted_y))  
    final_y_true[2] = numpy.concatenate((final_y_true[2], y_final_truth[test_indices]))  
    fold_index_col[2] = numpy.concatenate((fold_index_col[2], index_col[test_indices]))  
    fold_timestamp_col[2] = numpy.concatenate((fold_timestamp_col[2], timestamp_col[test_indices])) 
    
    fold_cm = confusion_matrix(y_final_truth[test_indices], predicted_y)
    fold_acc_array[2][int(n_folds_to_run-remained_fold)]=(float(fold_cm[0,0]+fold_cm[1,1]))/float(fold_cm.sum())
    
    print("\ncm[0,0]=%d, cm[1,1]=%d, cm.sum()=%d" %(fold_cm[0,0],fold_cm[1,1],fold_cm.sum()))
    print("\nCM(fold %d/%d): ACC = %s\n %s\n" % (n_folds_to_run-remained_fold+1, n_folds_to_run, fold_acc_array[2][int(n_folds_to_run-remained_fold)], fold_cm))
    
    f.write("\ncm[0,0]=%d, cm[1,1]=%d, cm.sum()=%d" %(fold_cm[0,0],fold_cm[1,1],fold_cm.sum()))
    f.write("\nCM(fold %d/%d): ACC = %s\n %s\n" % (n_folds_to_run-remained_fold+1, n_folds_to_run, fold_acc_array[2][int(n_folds_to_run-remained_fold)], fold_cm))
    
    remained_fold = remained_fold - 1.0
    

arr = numpy.array([fold_score_array[2]])
arr2 = numpy.array([fold_acc_array[2]])

string_to_print = '\n\n---Results(fold %d/%d)---\n\nAve. Score: %0.3f  (+/- %0.3f)\nAve. ACC: %0.3f  (+/- %0.3f)' % (n_folds_to_run-remained_fold+1, n_folds_to_run, numpy.mean(arr, axis=1),numpy.std(arr, axis=1),numpy.mean(arr2, axis=1),numpy.std(arr2, axis=1))
print(string_to_print)
f.write(string_to_print)

 
string_to_print = '\nFeature Importance/Ranking'
print(string_to_print)
f.write(string_to_print)

feature_importance_avg_1 = feature_importance_sum_1/n_folds_to_run
feature_importance_avg_2 = feature_importance_sum_2/n_folds_to_run

s_fold_avg_feature_importance = '\n\nRFOREST (average across all folds) - Feature Importance (percent - total:%s) \n%s' % (feature_importance_avg_2.sum(),zip(Header_names,feature_importance_avg_2))
s_fold_avg_feature_ranking = '\n\nRFOREST (average across all folds) - Feature Ranking \n%s' % sorted(zip(Header_names,feature_importance_avg_2), key=lambda x: x[1], reverse=True)
print(s_fold_avg_feature_importance+s_fold_avg_feature_ranking)
f.write(s_fold_avg_feature_importance+s_fold_avg_feature_ranking)


# Random Forest
'''
final_y_pred[2] = RFOREST.fit(X_final_train, y_final_train).predict(X_final_test)
string_to_print = '\n\nRFOREST (sb%d) - Accuracy: %s, How predicted (truth, predicted)?\n%s' % (subject_to_examine, RFOREST.fit(X_final_train, y_final_train).score(X_final_test, y_final_truth), zip(y_final_truth,final_y_pred[2]))
print(string_to_print)
f.write(string_to_print)
            
feature_importance = RFOREST.feature_importances_   
feature_importance = 100.0 * feature_importance 

string_to_print = '\n\nRFOREST (sb%d)- Feature IMPORTANCE (percent: %s) along with Header Name\n%s' % (subject_to_examine, feature_importance.sum(),zip(Header_names,feature_importance))
print(string_to_print)
f.write(string_to_print)  

string_to_print = '\n\nRFOREST (sb%d) - Feature RANKING (by importance percent)\n%s' % (subject_to_examine, sorted(zip(Header_names,feature_importance), key=lambda x: x[1], reverse=True))
print(string_to_print)
f.write(string_to_print)  
'''

# print out output (truth, predicted)
#numpy.savetxt(output_file_to_print, zip(index_col,timestamp_col,final_y_true[2],final_y_pred[2]), delimiter=',', newline='\n', fmt="%s") # numpy.savetxt(fname, X, fmt='%.18e', delimiter=' ', newline='\n', header='', footer='', comments='# ')[source]


numpy.savetxt(output_file_to_print, sorted(zip(fold_index_col[2],fold_timestamp_col[2],final_y_true[2],final_y_pred[2]), key=lambda x: x[0], reverse=False), delimiter=',', newline='\n', fmt="%s")
#numpy.savetxt(output_file_to_print, zip(fold_index_col[2],fold_timestamp_col[2],final_y_true[2],final_y_pred[2]), delimiter=',', newline='\n', fmt="%s")
#sorted(zip(Header_names,feature_importance), key=lambda x: x[1], reverse=True)

# Report Results

kfold = n_folds_to_run;

s_classifier ='\n\n------- Random Forest (subject %d) -------' % subject_to_examine

print(s_classifier)
f.write(s_classifier)

s_accuracy = '\nAccuracy: %0.3f (+/- %0.3f)' % (cross_validation.cross_val_score(RFOREST, X_final_test, ravel(y_final_truth), cv=kfold).mean(), cross_validation.cross_val_score(RFOREST, X_final_test, ravel(y_final_truth), cv=kfold).std() * 2)
s_fold_score = '\nEach fold score: %s' % cross_validation.cross_val_score(RFOREST, X_final_test, ravel(y_final_truth), cv=kfold)

string_to_print = 'Within driver %d(%d-fold cross validation): %s %s' % (subject_to_examine, kfold,s_accuracy,s_fold_score)
print(string_to_print)
f.write(string_to_print)


cm = confusion_matrix(final_y_true[2], final_y_pred[2])
s_conf_matrix = '\nConfusion matrix:\n %s' %cm

string_to_print = '\n\n -- driver %d (individual model) -- : %s' % (subject_to_examine,s_conf_matrix)
print(string_to_print)
f.write(string_to_print)

print(classification_report(final_y_true[2], final_y_pred[2], target_names=class_labels_for_CR))
f.write(classification_report(final_y_true[2], final_y_pred[2], target_names=class_labels_for_CR))

true_positives = [0,0,0,0,0] # five classes in 'interaction states', so five zeros
false_negatives = [0,0,0,0,0]
false_positives = [0,0,0,0,0]
true_negatives = [0,0,0,0,0]

for i in range(len(class_labels_for_CR)):
    for j in range(len(class_labels_for_CR)):
        if i == j:
            true_positives[i] += cm[i,j]
        else:
            false_negatives[i] += cm[i,j]   # FN(horizontal; sum of elements in the row aligned with TP: 1x4)  - varying j in cm
            false_positives[j] += cm[i,j]   # FP(vertical; sum of elements in the column aligned with TP: 4x1) - varying i in cm

string_to_print = "TP\tFN\tFP\tTN\tTotal\tTPR\tSPC\tNPV\tFPR\tFDR\tFNR\tACC\tprec\treca\tf1-sc\tClass\n"
print(string_to_print)
f.write(string_to_print)

for k in range(len(class_labels_for_CR)):
    true_negatives[k] = cm.sum()-true_positives[k]-false_negatives[k]-false_positives[k]
    total = true_positives[k]+false_negatives[k]+false_positives[k]+true_negatives[k]
    SJ_TPR = true_positives[k] / float(true_positives[k]+false_negatives[k])
    SJ_SPC = true_negatives[k] / float(false_positives[k]+true_negatives[k])
    SJ_NPV = true_negatives[k] / float(true_negatives[k]+false_negatives[k])
    SJ_FPR = false_positives[k] / float(false_positives[k]+true_negatives[k])
    SJ_FDR = false_positives[k] / float(false_positives[k]+true_positives[k])
    SJ_FNR = false_negatives[k] / float(false_negatives[k]+true_positives[k])
    SJ_ACC = float(true_positives[k]+true_negatives[k]) / float(cm.sum())
    SJ_precision = true_positives[k] / float(true_positives[k]+false_positives[k])
    SJ_recall = true_positives[k] / float(true_positives[k]+false_negatives[k])
    SJ_fscore = 2 * (SJ_precision * SJ_recall) / float(SJ_precision + SJ_recall)
    string_to_print = '%d\t%d\t%d\t%d\t%d\t%0.3f\t%0.3f\t%0.3f\t%0.3f\t%0.3f\t%0.3f\t%0.3f\t%0.3f\t%0.3f\t%0.3f\t%s\n' % (true_positives[k], false_negatives[k], false_positives[k], true_negatives[k], total, SJ_TPR, SJ_SPC, SJ_NPV, SJ_FPR, SJ_FDR, SJ_FNR, SJ_ACC, SJ_precision, SJ_recall, SJ_fscore, class_labels_for_CR[k])
    print(string_to_print)
    f.write(string_to_print)
    #print('%d\t%d\t%d\t%d\t%d\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%s' % (true_positives[k], false_negatives[k], false_positives[k], true_negatives[k], total, SJ_TPR, SJ_SPC, SJ_NPV, SJ_FPR, SJ_FDR, SJ_FNR, SJ_ACC, SJ_precision, SJ_recall, SJ_fscore, class_labels_for_CR[k]))
    #f.write('%d\t%d\t%d\t%d\t%d\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%s\n' % (true_positives[k], false_negatives[k], false_positives[k], true_negatives[k], total, SJ_TPR, SJ_SPC, SJ_NPV, SJ_FPR, SJ_FDR, SJ_FNR, SJ_ACC, SJ_precision, SJ_recall, SJ_fscore, class_labels_for_CR[k]))

f.close()

import winsound
winsound.Beep(100,500)