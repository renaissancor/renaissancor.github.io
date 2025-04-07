
int main()
{
	int i = 0;
	int* pInt = nullptr;

	{
		int a = 200;
		pInt = &a;
	}

	*pInt = 300;
	int k = *pInt;

	// switch case
	int check = 3;
	switch (check)
	{
	case 1:
	case 2:
	case 3:


		break;
	default:

		break;
	}

	if (check == 1 || check == 2 || check == 3)
	{

	}
	else
	{

	}

	return 0;
}