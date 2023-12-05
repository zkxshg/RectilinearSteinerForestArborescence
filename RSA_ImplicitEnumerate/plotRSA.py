#!/usr/bin/env python
# coding: utf-8

# In[1]:


import pandas as pd
import matplotlib.pyplot as plt


# In[2]:


# Read data from CSV file
df = pd.read_csv('./data/RSA_231205.csv')


# In[6]:


# Determine colors
colors = ['red' if is_omega else 'blue' for is_omega in df['isOmega']]

# Plot nodes
plt.scatter(df['coordinate_0'], df['coordinate_1'], c=colors)

# Plot edges
for _, row in df.iterrows():
    if pd.notna(row['predecessor_id']):
        pred = df[df['id'] == row['predecessor_id']].iloc[0]
        plt.plot([row['coordinate_0'], pred['coordinate_0']], [row['coordinate_1'], pred['coordinate_1']], 'b-')

    if pd.notna(row['successor_ids']):
        succ_ids = map(int, row['successor_ids'].split(';'))
        for succ_id in succ_ids:
            succ = df[df['id'] == succ_id].iloc[0]
            plt.plot([row['coordinate_0'], succ['coordinate_0']], [row['coordinate_1'], succ['coordinate_1']], 'r-')

plt.show()


# In[ ]:




