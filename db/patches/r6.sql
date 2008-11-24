update tblConfiguration set eng = 'r6', esp = 'r6', fra = 'r6' where var = 'update';
alter table tblLoansMaster ADD COLUMN `chk_process` INT NOT NULL DEFAULT 0 AFTER `xp_first_payment_date`;
insert into `tblLocalizedTexts` (`id` ,`msg_id` ,`eng` ,`esp` ,`fra`) values (NULL , 'prints', 'Prints', 'Impresiones', 'Prints');