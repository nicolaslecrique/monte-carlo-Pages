



std::vector<double> integrate(int N, double A, double B)
{
  double X, h, Iapp0, Iapp1, Iapp2, Iapp;
  int NN, i;

  // Etape 1
  h = (B - A) / N;

  // Etape 2
  Iapp0 = F(A) + F(B);
  Iapp1 = 0.0;
  Iapp2 = 0.0;

  // Etape 3
  NN = N -1;
  for (i=1; i<=NN; i++)
    {
      // Etape 4
      X = A + i*h;
      // Etape 5
      if ((i%2) == 0)
        Iapp2 = Iapp2 + F(X);
      else
        Iapp1 = Iapp1 + F(X);
    }

  // Etape 6
  Iapp = (Iapp0 + 2.0 * Iapp2 + 4.0 * Iapp1) * h / 3.0;

  // Etape 7
  return (Iapp);

}