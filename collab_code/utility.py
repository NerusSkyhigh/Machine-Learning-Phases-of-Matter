def dataframe_from_lattice_csv(fileName, lattice_side):
  df = pd.read_csv(fileName, names=['L', 'T', 'E']+['LP'+str(i) for i in range(lattice_side*lattice_side)] )

  df['lattice'] = df[df.columns[3:]].apply( lambda x: np.array(x), axis=1)
  df = df.drop(columns=['L']+['LP'+str(i) for i in range(lattice_side*lattice_side)])
  df['magnetisation']  = df['lattice'].apply(lambda x: x.sum()/x.shape[0])

  return df
