using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Turbo.Budget.SQLCE;
using Turbo.Budget.Types;

namespace Turbo.Budget.SQLCE.Test
{
    class Program
    {
        static void Main(string[] args)
        {

            ITurboBudgetRepository repository = new TurboBudgetSQLCERepository();

            //new List<ITurboBudgetAccount>()
            //{
            //    repository.CreateAccount("DEPOSIT", "Deposit", "Main Deposit Account"),
            //    repository.CreateAccount("AUTOMATIC", "Automatic", "Automatic Debits and Cheques"),
            //    repository.CreateAccount("PAYCHEQUE", "Paycheque", "Paycheque from Dynamic Risk"),
            //}.ForEach(a => repository.AddAccount(a));

            var accounts = repository.Accounts.ToList();

            Console.WriteLine("Accounts ---------------------------------------------------------------");
            accounts.ForEach(a => Console.WriteLine(a.Caption + " - '" + a.Description + "'"));
            Console.WriteLine("------------------------------------------------------------------------");


            //new List<ITurboBudgetTransaction>()
            //{
            //    repository.CreateTransaction(new DateTime(2015, 3,  6), accounts.Single(a => a.Name == "PAYCHEQUE"), accounts.Single(a => a.Name == "DEPOSIT"), 2500, "Paycheque"),
            //    repository.CreateTransaction(new DateTime(2015, 3, 15), accounts.Single(a => a.Name == "DEPOSIT"), accounts.Single(a => a.Name == "AUTOMATIC"), 1376, "Transfer to Automatic Account"),
            //}.ForEach(t => repository.AddTransaction(t));

            var transactions = repository.Transactions.ToList();

            Console.WriteLine("Transactions -----------------------------------------------------------");
            transactions.ForEach(t => Console.WriteLine(t.TimeStamp + ": " + t.Amount + " From " + t.FromAccountId + " to " + t.ToAccountId + " - '" + t.Description + "'"));
            Console.WriteLine("------------------------------------------------------------------------");



            Console.ReadLine();
        }
    }
}
