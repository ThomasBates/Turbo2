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

            repository.Accounts.ToList().ForEach(a => { Console.WriteLine(a.Caption); });

            Console.ReadLine();
        }
    }
}
