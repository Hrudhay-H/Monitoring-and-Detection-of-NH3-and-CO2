from IPython import get_ipython
from IPython.display import display
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.model_selection import train_test_split, GridSearchCV
from sklearn.preprocessing import StandardScaler, PolynomialFeatures
from sklearn.metrics import accuracy_score, classification_report, confusion_matrix
from sklearn.pipeline import Pipeline
from xgboost import XGBClassifier
from sklearn.preprocessing import LabelEncoder

# Loading the Dataset
df = pd.read_csv("merged_egg_production_dataset.csv")

# Features and target
X = df[['Ammonia (ppm)', 'CO2 (ppm)', 'Temperature (°C)', 'Humidity (%)']]
y = df['Egg Production Category']

# Encode target labels
le = LabelEncoder()
y_encoded = le.fit_transform(y)

# Split data with stratification
X_train, X_test, y_train, y_test = train_test_split(X, y_encoded, test_size=0.2, random_state=42, stratify=y_encoded)

# Create polynomial features
poly = PolynomialFeatures(degree=2, interaction_only=True, include_bias=False)
X_train_poly = poly.fit_transform(X_train)
X_test_poly = poly.transform(X_test)

# Pipeline for XGBoost
xgb_pipeline = Pipeline([
    ('scaler', StandardScaler()),
    ('clf', XGBClassifier(random_state=42, use_label_encoder=False, eval_metric='mlogloss'))
])

# Expanded hyperparameter grid for XGBoost
xgb_param_grid = {
    'clf__n_estimators': [100, 200, 300],
    'clf__max_depth': [3, 5, 7],
    'clf__learning_rate': [0.01, 0.1, 0.2],
    'clf__subsample': [0.7, 0.8, 1.0],
    'clf__colsample_bytree': [0.7, 0.8, 1.0],
    'clf__gamma': [0, 0.1, 0.2]
}

# GridSearchCV for hyperparameter tuning
xgb_grid = GridSearchCV(xgb_pipeline, xgb_param_grid, cv=5, scoring='accuracy', n_jobs=-1)
xgb_grid.fit(X_train_poly, y_train)  # Use polynomial features

# Best XGBoost model
xgb_best = xgb_grid.best_estimator_

# Test set evaluation
y_pred_xgb = xgb_best.predict(X_test_poly)  # Use polynomial features
print("\nXGBoost Test Accuracy:", accuracy_score(y_test, y_pred_xgb) * 100)
print("Classification Report:\n", classification_report(y_test, y_pred_xgb, target_names=le.classes_.astype(str)))
print("Confusion Matrix:\n", confusion_matrix(y_test, y_pred_xgb))

# Example prediction (using polynomial features)
new_data = [[10, 500, 25, 60]]
new_data_poly = poly.transform(new_data)
new_data_scaled = xgb_best.named_steps['scaler'].transform(new_data_poly)
xgb_prediction = xgb_best.named_steps['clf'].predict(new_data_scaled)
print("\nPredicted Egg Production Level for [10, 500, 25, 60]:", le.inverse_transform(xgb_prediction)[0])

# Optional: Print best parameters
# print("\nBest XGBoost Parameters:", xgb_grid.best_params_)
