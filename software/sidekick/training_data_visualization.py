import numpy as np
import regex as re
import seaborn as sns
from matplotlib import pyplot as plt
from sklearn import metrics
from sklearn import preprocessing
from sklearn.model_selection import GridSearchCV
from sklearn.model_selection import train_test_split
from sklearn.svm import SVC

data = open("sign_buddy_training_data.txt", "r").read()

proto_strings = re.compile("(?:^.*$\n?){1,32}", re.M).findall(data)
del proto_strings[-1]

x_data = []
y_data = []
for proto in proto_strings:
    list1 = []
    proto = proto.replace(" ", "")
    for line in proto.splitlines():
        list1.append(re.compile("^[a-z0-9_]+:\K(?!//).*", re.M).findall(line))
    list2 = [x for x in list1 if x != []]
    y_data.append(list2[0][0])
    del list2[0]
    del list2[9]
    for i in range(len(list2)):
        if i in range(8):
            list2[i] = int(list2[i][0])
        else:
            if list2[i][0] == 'false':
                list2[i] = -1
            else:
                list2[i] = 1
    x_data.append(list2)

data = np.zeros((240, 21))
for i in range(data.shape[0]):
    data[i] = np.array(x_data[i])
classification = np.array(y_data)

data_normalized, norms = preprocessing.normalize(data, axis=0, return_norm=True)  # Norms used to normalize live data
covariance_matrix = np.cov(data_normalized.T)
eigenvalues, eigenvectors = np.linalg.eig(covariance_matrix)  # Get eigenvectors
print("eigenvector: \n", eigenvectors, "\n")
print("eigenvalues: \n", eigenvalues, "\n")
projection_matrix = (eigenvectors.T[:][:3]).T  # Get first two components
print("first two components: \n", projection_matrix, "\n")
pca = data_normalized.dot(projection_matrix)  # Perform dimension reduction using PCA components
print("pca: \n", pca, "\n")

le = preprocessing.LabelEncoder()
le.fit(classification)
labels = le.transform(classification)  # Get integer labels from letters

fig = plt.figure(figsize=(12, 12))
ax = fig.add_subplot(projection='3d')
ax.scatter(pca[:, 0], pca[:, 1], pca[:, 2], c=labels)
plt.show()

x_train, x_test, y_train, y_test = train_test_split(data_normalized, labels, test_size=1 / 2, random_state=42)
parameters = {'kernel': ('linear', 'rbf'), 'C': [0.01, 0.1, 1, 10, 100, 1000, 10000],
              'gamma': [0.001, 0.01, 0.1, 1, 10, 100, 1000, 10000]}
clf = GridSearchCV(SVC(), parameters)
clf.fit(x_train, y_train)
print(clf.best_score_)
print(clf.best_estimator_)
print(clf.best_params_)
model = SVC(kernel='rbf', gamma=10, C=1000)
model.fit(x_train, y_train)
y_pred = model.predict(x_train)
print("\nCONFUSION MATRIX TRAIN")
confusion_matrix_train = metrics.confusion_matrix(y_train, y_pred)
print(confusion_matrix_train)
print(metrics.classification_report(y_train, y_pred))
unique, counts = np.unique(classification, return_counts=True)
sns.heatmap(confusion_matrix_train, square=True, annot=True, fmt='d', cbar=False, xticklabels=unique,
            yticklabels=unique)
plt.xlabel('Prediction')
plt.ylabel('Actual')
plt.title('Confusion matrix for training data')
plt.show()
y_pred_test = model.predict(x_test)
print("CONFUSION MATRIX TEST")
confusion_matrix_test = metrics.confusion_matrix(y_test, y_pred_test)
print(confusion_matrix_test)
print(metrics.classification_report(y_test, y_pred_test))
sns.heatmap(confusion_matrix_test, square=True, annot=True, fmt='d', cbar=False, xticklabels=unique,
            yticklabels=unique)
plt.xlabel('Prediction')
plt.ylabel('Actual')
plt.title('Confusion matrix for test data')
plt.show()
