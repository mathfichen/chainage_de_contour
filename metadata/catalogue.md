## SWHAPPE catalogue.md Template

Here we propose a template for catalogue<span>.md for SWHAPPE.

Each Item in the `raw_materials` and `additional_materials` folders of Workbench should have a corresponding record on the `catalogue.md` with the structure below..

Please note that:
* Name and Surname of actors should be linked to their paragraph in [actors.md](./actors.md) file;
* Items should be linked to the file [inside the raw materials repository](./raw_matherials/) or [inside the additional materials repository](./additional_matherials/);
* On the [second part of the Catalogue](./catalogue.md#SW_NAME-Catalogue-Tree) should be copied the result of the command `tree -a raw_materials additional_materials ` ;
* *Notes:* are optional; they should contains `Additional materials.` for additional materials
* *Warehouse:* is optional - should be used only when a physical warehouse is used to store material taken from the *origin*; .

Example of Actor link:
~~~
[Name Surname](./actors.md#name-surname)
~~~
Example of Item links:
~~~
[Item Name](./raw_materials/example_file.zip)
[Additional Materials Item Name](./additional_materials/example.ppt)
~~~


# SW_NAME Catalogue


* **[Item Name](./raw_materials/example_file.zip)**
  * *Origin:* 
  * *Warehouse:* 
  * *Authors:* [Name Surname](./actors.md#name-surname)
  * *Date:* 1970/01/25 (circa) 
  * *Collectors:* [Name Surname](./actors.md#name-surname)
  * *Description:* 
  * *Notes:* 
  
* **[Additional Material Item Name](./additional_materials/example.ppt)**
  * *Origin:* 
  * *Authors:* [Name Surname](./actors.md#name-surname)
  * *Date:* 1979/05/20 
  * *Collectors:* [Name Surname](./actors.md#name-surname)
  * *Description:* 
  * *Notes:* Additional materials;

# SW_NAME Catalogue Tree


result of `tree -a raw_materials additional_materials` command.
