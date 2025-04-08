struct maillon
	{
	struct maillon *succ;
	union
		{
		struct
			{	/* tete de chaine */
			struct maillon *dernier;	/* dernier maillon */
			int numero;		      /* numero de la chaine */
			unsigned char mode ;		/* OPEN ou CLOSE, CONT */
			unsigned char magic ;			/* magic number */
			} t ;

		struct
			{	/* extension de la tete de chaine */
			int famille[4];
			} e ;

		struct ms
			{	/* maillon simple */
			int data[3];
			} m;
		} u ;
	};
